// Kurs_Project.cpp: определяет точку входа для приложения.
//

#include "resource.h"
#include "stdafx.h"
#include "ResourceMonitor.h"
#include <TlHelp32.h>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <psapi.h>
#include <tchar.h>
#include <crtversion.h>


#define MAX_LOADSTRING 100
#define UPDATE_TIMER_ID 3001
#define UPDATE_TIMER_ID1 3002
#define UPDATE_TIMER_ID2 3003
#define UPDATE_TIMER_ID3 3004
#define ARRAY_SIZE 1024
#define INFO_BUFFER_SIZE 32767

#define MAX_LOADSTRING 100

// Global vars
HINSTANCE hInst;								// current
HWND hDlg;

// Function defenition
INT_PTR CALLBACK	SystemMonitor(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK UpdateTimerFunctionP(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK UpdateTimerFunctionS(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK UpdateTimerFunctionD(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK UpdateTimerFunctionM(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void PrintINF_PC(HWND hWnd);
void PrintMemoryInfo(HWND hWnd, DWORD processID);
void DrawGraph(HWND hWnd, int percent);
char* ProcessIdToName(DWORD processId);


wchar_t* buf;
wchar_t* wbuf;
wchar_t * ItoT(int i)
{
	if (!buf) free(buf);
	buf = (wchar_t*)calloc(10, sizeof(wchar_t));
	wsprintf(LPSTR(buf), "%d", i);
	return buf;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	MSG msg;
	hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_SYSTEM_MONITOR), 0, SystemMonitor);
	ShowWindow(hDlg, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, 0, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
#pragma warning(push)
#pragma warning(disable:4996)
INT_PTR CALLBACK SystemMonitor(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HBRUSH hbrush = CreateSolidBrush(RGB(188, 143, 143));

	switch (message)
	{
	case WM_INITDIALOG:
		PrintINF_PC(hDlg); // Computer Information
		UpdateTimerFunctionS(hDlg, 0, 0, 0);
		SetTimer(hDlg, UPDATE_TIMER_ID1, 1000, UpdateTimerFunctionS);
		UpdateTimerFunctionP(hDlg, 0, 0, 0);
		SetTimer(hDlg, UPDATE_TIMER_ID, 10000, UpdateTimerFunctionP);
		UpdateTimerFunctionD(hDlg, 0, 0, 0); // DriversPanel
		SetTimer(hDlg, UPDATE_TIMER_ID2, 10000, UpdateTimerFunctionD);
		UpdateTimerFunctionM(hDlg, 0, 0, 0); // Process panel
		SetTimer(hDlg, UPDATE_TIMER_ID3, 30000, UpdateTimerFunctionM);
		break;

		return (INT_PTR)TRUE;

	case WM_CTLCOLORDLG:
		return (LONG)hbrush;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LONG)hbrush;
	}

	case WM_COMMAND:
		switch (wParam)
		{
		case IDCANCEL:
			KillTimer(hDlg, UPDATE_TIMER_ID1);
			KillTimer(hDlg, UPDATE_TIMER_ID);
			KillTimer(hDlg, UPDATE_TIMER_ID2);
			KillTimer(hDlg, UPDATE_TIMER_ID3);
			PostQuitMessage(0);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void PrintINF_PC(HWND hWnd) // Get System Info
{
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	//User Name
	bufCharCount = INFO_BUFFER_SIZE;
	if (GetUserName(infoBuf, &bufCharCount))
	{
		SendMessage(GetDlgItem(hWnd, IDC_EDIT4), WM_SETTEXT, NULL, (LPARAM)infoBuf);
	}
	//Computer Name
	bufCharCount = INFO_BUFFER_SIZE;
	if (GetComputerName(infoBuf, &bufCharCount))
	{
		SendMessage(GetDlgItem(hWnd, IDC_EDIT5), WM_SETTEXT, NULL, (LPARAM)infoBuf);
	}

}

VOID CALLBACK UpdateTimerFunctionS(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	PERFORMACE_INFORMATION pi;
	char buf[1024];
	int  percent;
	double PhysicalUsage;
	if (GetPerformanceInfo(&pi, sizeof(pi)))
	{
		percent = pi.PhysicalAvailable * 100 / pi.PhysicalTotal;
		DrawGraph(hDlg, percent);
	}
}

void DrawGraph(HWND hWnd, int percent)
{
	HDC hdc = GetDC(hWnd); // Get DC
	HGDIOBJ hpenOld, hbrushOld;
	HBRUSH hbrush;

	hbrush = CreateSolidBrush(RGB(0, 255, 0));
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	Rectangle(hdc, 705, 70, 755, 170);

	hbrush = CreateSolidBrush(RGB(0, 0, 0));
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	Rectangle(hdc, 705, 70, 755, percent + 70);
	ReleaseDC(hWnd, hdc);
}

VOID CALLBACK UpdateTimerFunctionP(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	HWND hListBox = GetDlgItem(hWnd, IDC_LIST1);
	SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	process.dwSize = sizeof(PROCESSENTRY32);
	char *buffer = new char[1024];
	int kolvoproc = 0;
	if (Process32First(hSnapshot, &process))
	{
		do
		{
			sprintf_s(buffer, 1024, "%u %s", process.th32ProcessID, process.szExeFile);
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
			kolvoproc++;
		} while (Process32Next(hSnapshot, &process));
	}
	//SendMessage(GetDlgItem(hWnd, IDC_EDIT16), WM_SETTEXT, NULL, (LPARAM)ItoT(kolvoproc));
	delete buffer;
	CloseHandle(hSnapshot);
}

VOID CALLBACK UpdateTimerFunctionD(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) //Driver panel
{
	HWND hListBox = GetDlgItem(hWnd, IDC_LIST2);
	LPVOID drivers[ARRAY_SIZE];
	DWORD cbNeeded;
	int cDrivers, i;
	char *buffer = new char[1024];

	SendMessage(hListBox, LB_RESETCONTENT, NULL, NULL);
	if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
	{
		TCHAR szDriver[ARRAY_SIZE];

		cDrivers = cbNeeded / sizeof(drivers[0]);

		SendMessage(GetDlgItem(hWnd, IDC_EDIT17), WM_SETTEXT, NULL, (LPARAM)ItoT(cDrivers));
		for (i = 0; i < cDrivers; i++)
		{
			if (GetDeviceDriverBaseName(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0])))
			{
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)szDriver);
			}
		}
	}
	else
	{
		sprintf_s(buffer, 1024, "%s %d", "Error print drivers; sire of array", cbNeeded / sizeof(LPVOID));
		MessageBox(hWnd, buffer, "Error", NULL);
	}
	delete buffer;
}

VOID CALLBACK UpdateTimerFunctionM(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) //Process Panel
{
	PERFORMACE_INFORMATION pi;
	char buf[1024];
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (GetPerformanceInfo(&pi, sizeof(pi))) // thread count
	{
		sprintf(buf, "%i", pi.ThreadCount);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT18), WM_SETTEXT, NULL, (LPARAM)buf);
	}

	SendMessage(GetDlgItem(hWnd, IDC_LIST3), LB_RESETCONTENT, NULL, NULL);
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		MessageBox(hWnd, "Can't get the process List", "Error", NULL);
	}

	cProcesses = cbNeeded / sizeof(DWORD);

	for (i = 0; i < cProcesses; i++)
	{
		PrintMemoryInfo(hWnd, aProcesses[i]);
	}
	SendMessage(GetDlgItem(hWnd, IDC_EDIT16), WM_SETTEXT, NULL, (LPARAM)ItoT(cProcesses));
}

void PrintMemoryInfo(HWND hWnd, DWORD processID)// print process information
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;
	char buff[1024];
	int i = 0;
	int s = 0;

	char* filename = strrchr(ProcessIdToName(processID), '\\');
	if (filename == NULL) filename = (char*)"[System Process]";
	else filename++;
	i += sprintf_s(buff + i, 1024, "%s",filename);
	while (i < 34)
	{
		i += sprintf(buff + i, "%s", " ");
	}

	i += sprintf(buff + i, "%d", processID);
	while (i < 43)
	{
		i += sprintf(buff + i, "%s", " ");
	}

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return;

	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) 
	{
		i += sprintf(buff + i, "%s %d", " ", pmc.PageFaultCount);
		while (i < 56)
		{
			i += sprintf(buff + i, "%s", " ");
		}
		i += sprintf(buff + i, "%s %i", " ", pmc.PeakWorkingSetSize / (1024));
		while (i < 67)
		{
			i += sprintf(buff + i, "%s", " ");
		}
		i += sprintf(buff + i, "%s %i", " ", pmc.WorkingSetSize / (1024));
		while (i < 80)
		{
			i += sprintf(buff + i, "%s", " ");
		}
		i += sprintf(buff + i, "%s %i", " ", pmc.QuotaPeakPagedPoolUsage / (1024));
		while (i < 96)
		{
			i += sprintf(buff + i, "%s", " ");
		}
		i += sprintf(buff + i, "%s %i", " ", pmc.QuotaPagedPoolUsage / (1024));
		while (i < 111)
		{
			i += sprintf(buff + i, "%s", " ");
		}
		i += sprintf(buff + i, "%s %i", " ", pmc.QuotaPeakNonPagedPoolUsage / (1024));
		while (i < 127)
		{
			i += sprintf(buff + i, "%s", " ");
		}
		i += sprintf(buff + i, "%s %i", " ", pmc.QuotaNonPagedPoolUsage / (1024));
		while (i < 138)
		{
			i += sprintf(buff + i, "%s", " ");
		}
		i += sprintf(buff + i, "%s %i", " ", pmc.PagefileUsage / (1024));
		while (i < 148)
		{
			i += sprintf(buff + i, "%s", " ");
		}
		i += sprintf(buff + i, "%s %i", " ", pmc.PeakPagefileUsage / (1024));
	}
	SendMessage(GetDlgItem(hWnd, IDC_LIST3), WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FIXED_FONT), NULL);
	SendMessage(GetDlgItem(hWnd, IDC_LIST3), LB_ADDSTRING, 0, (LPARAM)buff);
	CloseHandle(hProcess);
}

char* ProcessIdToName(DWORD processId) // get process name
{
	CHAR ret[1024];
	HANDLE handle = OpenProcess(
		PROCESS_QUERY_LIMITED_INFORMATION,
		FALSE,
		processId /* This is the PID, you can find one from windows task manager */
	);
	if (handle)
	{
		DWORD buffSize = 1024;
		CHAR buffer[1024];
		if (QueryFullProcessImageNameA(handle, 0, buffer, &buffSize))
		{
			strcpy(ret, buffer);
		}
		else
		{
			printf("Error GetModuleBaseNameA : %lu", GetLastError());
		}
		CloseHandle(handle);
	}
	else
	{
		printf("Error OpenProcess : %lu", GetLastError());
	}
	return ret;
}

#pragma warning(pop)


