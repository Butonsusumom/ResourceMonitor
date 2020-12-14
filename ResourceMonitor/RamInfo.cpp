#include "stdafx.h"
#include "Base.h"
#include "Converting.h"
#include "Graph.h"

#pragma warning(disable:4996)
int countRam(HWND hWnd, HWND hDlg ,MEMORYSTATUSEX memInfo, int totalEdit, int availableEdit, int usageEdit, int percentEdit)
{
	int totalRAM;
	int availableRAM;
	int RAMUsage;
	int percent;
	char buff[7];

	totalRAM = memInfo.ullTotalPhys / (1024 * 1024);
	SendMessage(GetDlgItem(hWnd, totalEdit), WM_SETTEXT, NULL, (LPARAM)ItoT(totalRAM));
	availableRAM = memInfo.ullAvailPhys / (1024 * 1024);
	SendMessage(GetDlgItem(hWnd, availableEdit), WM_SETTEXT, NULL, (LPARAM)ItoT(availableRAM));
	RAMUsage = totalRAM - availableRAM;
	SendMessage(GetDlgItem(hWnd, usageEdit), WM_SETTEXT, NULL, (LPARAM)ItoT(RAMUsage));
	percent = RAMUsage * 100 / totalRAM;
	DrawGraph(hDlg, percent, 230, 100, 247, 123);
	sprintf(buff, "%i%%", percent);
	SendMessage(GetDlgItem(hWnd, percentEdit), WM_SETTEXT, NULL, (LPARAM)buff);

	return percent;
}