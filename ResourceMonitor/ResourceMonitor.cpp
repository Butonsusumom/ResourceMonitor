#include "stdafx.h"
#include "Base.h"
#include "ResourceMonitor.h"
#include "ComputerInfo.h"
#include "DriverInfo.h"
#include "Converting.h"
#include "CpuInfo.h"
#include "RamInfo.h"
#include "VirtmemInfo.h"
#include "ProcInfo.h"

#define MAX_LOADSTRING 100
#define UPDATE_TIMER_ID 3001
#define UPDATE_TIMER_ID1 3002
#define UPDATE_TIMER_ID2 3003
#define UPDATE_TIMER_ID3 3004

#define MAX_LOADSTRING 100

// Global vars
HINSTANCE hInst;								// current
HWND hDlg;
//for CPU avarage
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
MEMORYSTATUSEX memInfo;
PDH_HQUERY cpuQuery;
PDH_HCOUNTER cpuTotal;

// Function defenition
INT_PTR CALLBACK	SystemMonitor(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK UpdateTimerFunctionOverview(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK UpdateTimerFunctionProcesses(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK UpdateTimerFunctionDrivers(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

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
	HBRUSH hbrush = CreateSolidBrush(RGB(194, 179, 179));

	switch (message)
	{
	case WM_INITDIALOG:
		PrintINF_PC(hDlg, IDC_EDIT4, IDC_EDIT5); // Computer Information
		initCPU(memInfo, cpuQuery, cpuTotal);
		SetTimer(hDlg, UPDATE_TIMER_ID1, 1000, UpdateTimerFunctionOverview);//Overview Panel
		SetTimer(hDlg, UPDATE_TIMER_ID2, 1000, UpdateTimerFunctionDrivers);// DriversPanel

		SetTimer(hDlg, UPDATE_TIMER_ID3, 1000, UpdateTimerFunctionProcesses);// Process panel
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
		case WM_ERASEBKGND:
			return TRUE;
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

VOID CALLBACK UpdateTimerFunctionDrivers(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	GetDriverInfo(hWnd,IDC_LIST2, IDC_EDIT17);
}

VOID CALLBACK UpdateTimerFunctionOverview(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) //Overview panel
{
	//HWND hGroup = GetDlgItem(hWnd, IDC_GROUPBOX19);;
	GlobalMemoryStatusEx(&memInfo);

	countVirtmem(hWnd, hDlg, memInfo, IDC_EDIT22, IDC_EDIT23, IDC_EDIT24, IDC_EDIT28);
	countRam(hWnd, hDlg, memInfo, IDC_EDIT19, IDC_EDIT20, IDC_EDIT21, IDC_EDIT29);
	countCPU(hWnd, hDlg, cpuQuery, cpuTotal, IDC_EDIT26);
}

VOID CALLBACK UpdateTimerFunctionProcesses(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) //Process Panel
{
	getProcInfo(hWnd, IDC_LIST3, IDC_EDIT16, IDC_EDIT18);
}

#pragma warning(pop)


