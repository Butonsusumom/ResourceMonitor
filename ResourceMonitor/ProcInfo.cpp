#include "stdafx.h"
#include "Base.h"
#include "Converting.h"

#pragma warning(disable:4996)

void PrintMemoryInfo(HWND hWnd, DWORD processID);
int printString(char buff[1024], int i, int number);
int printSpace(char buff[1024], int i, int number);

void getProcInfo(HWND hWnd, int list, int editProc, int editThred)
{
	HWND hListBox = GetDlgItem(hWnd, list);
	PERFORMACE_INFORMATION pi;
	char buf[1024];
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;
	int index = SendMessage(hListBox, LB_GETTOPINDEX, NULL, NULL);

	SendMessage(hListBox, WM_SETREDRAW, FALSE, NULL);
	SendMessage(hListBox, LB_RESETCONTENT, NULL, NULL);
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		MessageBox(hWnd, "Can't get the process List", "Error", NULL);
	}

	cProcesses = cbNeeded / sizeof(DWORD);
	for (i = 0; i < cProcesses; i++) 
	{
		PrintMemoryInfo(hWnd, aProcesses[i]);
	}
	SendMessage(hListBox, LB_SETTOPINDEX, (WPARAM)index, 0);
	SendMessage(hListBox, WM_SETREDRAW, TRUE, NULL);

	SendMessage(GetDlgItem(hWnd, editProc), WM_SETTEXT, NULL, (LPARAM)ItoT(cProcesses));// process count

	if (GetPerformanceInfo(&pi, sizeof(pi))) // thread count
	{
		sprintf(buf, "%i", pi.ThreadCount);
		SendMessage(GetDlgItem(hWnd, editThred), WM_SETTEXT, NULL, (LPARAM)buf);
	}

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
	i += sprintf_s(buff + i, 1024, "%s", filename);
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
		i += printString(buff, i, pmc.PageFaultCount);
		i = printSpace(buff, i, 56);
		i += printString(buff, i, pmc.PeakWorkingSetSize / (1024));
		i = printSpace(buff, i, 67);
		i += printString(buff, i, pmc.WorkingSetSize / (1024));
		i = printSpace(buff, i, 80);
		i += printString(buff, i, pmc.QuotaPeakPagedPoolUsage / (1024));
		i = printSpace(buff, i, 88);
		i += printString(buff, i, pmc.QuotaPagedPoolUsage / (1024));
		i = printSpace(buff, i, 96);
		i += printString(buff, i, pmc.QuotaPeakNonPagedPoolUsage / (1024));
		i = printSpace(buff, i, 104);
		i += printString(buff, i, pmc.QuotaNonPagedPoolUsage / (1024));
		i = printSpace(buff, i, 112);
		i += printString(buff, i, pmc.PagefileUsage / (1024));
		i = printSpace(buff, i, 124);
		i += printString(buff, i, pmc.PeakPagefileUsage / (1024));
	}
	SendMessage(GetDlgItem(hWnd, IDC_LIST3), WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FIXED_FONT), NULL);
	SendMessage(GetDlgItem(hWnd, IDC_LIST3), LB_ADDSTRING, 0, (LPARAM)buff);
	CloseHandle(hProcess);
}

int printString(char buff[1024],int i, int number) 
{
	return sprintf(buff + i, "%s %d", " ", number);
}

int printSpace(char buff[1024], int i, int number)
{
	while (i < number)
	{
		i += sprintf(buff + i, "%s", " ");
	}
	return i;
}