#include "stdafx.h"
#include "Base.h"
#include "Graph.h"

#pragma warning(disable:4996)
void initCPU(MEMORYSTATUSEX &memInfo, PDH_HQUERY &cpuQuery, PDH_HCOUNTER &cpuTotal)
{
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddEnglishCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);
}

void countCPU(HWND hWnd, HWND hDlg, PDH_HQUERY &cpuQuery, PDH_HCOUNTER &cpuTotal,int edit)
{
	double CPUUsage;
	PDH_FMT_COUNTERVALUE counterVal;
	char buff[7];

	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

	CPUUsage = counterVal.doubleValue;
	DrawGraph(hDlg, CPUUsage, 230, 230, 247, 257);
	sprintf(buff, "%3.0f%%", CPUUsage);
	SendMessage(GetDlgItem(hWnd, edit), WM_SETTEXT, NULL, (LPARAM)buff);
}
