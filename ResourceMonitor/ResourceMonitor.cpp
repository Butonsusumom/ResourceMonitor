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
#include "Diagram.h"
#include "Graph.h"

#define MAX_LOADSTRING 100
#define UPDATE_TIMER_ID 3001
#define UPDATE_TIMER_ID1 3002
#define UPDATE_TIMER_ID2 3003
#define UPDATE_TIMER_ID3 3004
#define UPDATE_TIMER_ID4 3005

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
// Vatiables for graphic
int CPUcurr;
int CPUprev=0;
int RAMcurr;
int RAMprev=0;
int Virtcurr;
int Virtprev=0;
COLORREF fill = RGB(217, 209, 209);
COLORREF back =  RGB(255, 255, 255);
COLORREF graph = RGB(178, 163, 163);

// Function defenition
INT_PTR CALLBACK	SystemMonitor(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK UpdateTimerFunctionOverview(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK UpdateTimerFunctionProcesses(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK UpdateTimerFunctionDrivers(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
VOID CALLBACK UpdateTimerFunctionGraphic(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

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
		SetTimer(hDlg, UPDATE_TIMER_ID4, 1000, UpdateTimerFunctionGraphic); //Graphic panel

		break;

		return (INT_PTR)TRUE;

	case WM_PAINT:
		DrawRect(hDlg, 300, 70, 600, 220, back);
		Rectangle(GetDC(hDlg), 300-2, 70-2, 600+2, 220+2);

		DrawRect(hDlg, 680, 70, 980, 220, back);
		Rectangle(GetDC(hDlg), 680 - 2, 70 - 2 , 980 + 2, 220 + 2);

		DrawRect(hDlg, 1060, 70, 1360, 220, back);
		Rectangle(GetDC(hDlg), 1060 - 2, 70 - 2, 1360 + 2, 220 + 2);
	break;

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
			KillTimer(hDlg, UPDATE_TIMER_ID2);
			KillTimer(hDlg, UPDATE_TIMER_ID3);
			KillTimer(hDlg, UPDATE_TIMER_ID4);
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
	GlobalMemoryStatusEx(&memInfo);

	Virtprev = Virtcurr;
	RAMprev = RAMcurr;
	CPUprev = CPUcurr;

	Virtcurr = countVirtmem(hWnd, hDlg, memInfo, IDC_EDIT22, IDC_EDIT23, IDC_EDIT24, IDC_EDIT28);
	RAMcurr = countRam(hWnd, hDlg, memInfo, IDC_EDIT19, IDC_EDIT20, IDC_EDIT21, IDC_EDIT29);
	CPUcurr = countCPU(hWnd, hDlg, cpuQuery, cpuTotal, IDC_EDIT26);
}

VOID CALLBACK UpdateTimerFunctionProcesses(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) //Process Panel
{
	getProcInfo(hWnd, IDC_LIST3, IDC_EDIT16, IDC_EDIT18);
}

VOID CALLBACK UpdateTimerFunctionGraphic(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) //Graphic Panel
{
	DrawDiagram(hWnd, 300, 70, 600, 220, RAMprev, RAMcurr, 2, back, graph, fill);
	DrawDiagram(hWnd, 680, 70, 980, 220, Virtprev, Virtcurr, 2, back, graph, fill);
	DrawDiagram(hWnd, 1060, 70, 1360, 220, CPUprev, CPUcurr, 2, back, graph, fill);
}
#pragma warning(pop)


