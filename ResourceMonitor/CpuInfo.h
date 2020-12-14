#pragma once
#include "base.h"

void initCPU(MEMORYSTATUSEX &memInfo, PDH_HQUERY &cpuQuery, PDH_HCOUNTER &cpuTotal);

int countCPU(HWND hWnd, HWND hDlg, PDH_HQUERY &cpuQuery, PDH_HCOUNTER &cpuTotal, int edit);