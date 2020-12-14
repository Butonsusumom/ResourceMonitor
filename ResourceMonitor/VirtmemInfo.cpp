#include "stdafx.h"
#include "Base.h"
#include "Graph.h"
#include "Converting.h"

#pragma warning(disable:4996)
int countVirtmem(HWND hWnd, HWND hDlg, MEMORYSTATUSEX memInfo, int totalEdit, int availableEdit, int usageEdit, int percentEdit)
{
	int totalVirtualMem;
	int availableVirtualMem;
	int virtualMemUsage;
	int percent;
	char buff[7];

	totalVirtualMem = memInfo.ullTotalPageFile / (1024 * 1024);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT22), WM_SETTEXT, NULL, (LPARAM)ItoT(totalVirtualMem));
	availableVirtualMem = memInfo.ullAvailPageFile / (1024 * 1024);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT23), WM_SETTEXT, NULL, (LPARAM)ItoT(availableVirtualMem));
	virtualMemUsage = totalVirtualMem - availableVirtualMem;
	SendMessage(GetDlgItem(hWnd, IDC_EDIT24), WM_SETTEXT, NULL, (LPARAM)ItoT(virtualMemUsage));
	percent = virtualMemUsage * 100 / totalVirtualMem;
	DrawGraph(hDlg, percent, 230, 187, 247, 210);
	sprintf(buff, "%i%%", percent);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT28), WM_SETTEXT, NULL, (LPARAM)buff);

	return percent;
}