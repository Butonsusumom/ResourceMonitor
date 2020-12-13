#include "stdafx.h"
#include "Base.h"
#include "Converting.h"

#define ARRAY_SIZE 1024
void GetDriverInfo(HWND hWnd, int list, int edit) //Driver panel
{
	HWND hListBox = GetDlgItem(hWnd,list);// For Drivers
	LPVOID drivers[ARRAY_SIZE];
	DWORD cbNeeded;
	int cDrivers, i;
	char *buffer = new char[1024];
	int index = SendMessage(hListBox, LB_GETTOPINDEX, NULL, NULL);
	
	if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
	{
		TCHAR szDriver[ARRAY_SIZE];

		cDrivers = cbNeeded / sizeof(drivers[0]);
		SendMessage(GetDlgItem(hWnd, edit), WM_SETTEXT, NULL, (LPARAM)ItoT(cDrivers));

		SendMessage(hListBox, WM_SETREDRAW, FALSE, NULL);
		SendMessage(hListBox, LB_RESETCONTENT, NULL, NULL);
		for (i = 0; i < cDrivers; i++)
		{
			if (GetDeviceDriverBaseName(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0])))
			{
				SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)szDriver);
			}
		}
		SendMessage(hListBox, LB_SETTOPINDEX, (WPARAM)index, 0);
		SendMessage(hListBox, WM_SETREDRAW, TRUE, NULL);
	}
	else
	{
		sprintf_s(buffer, 1024, "%s %d", "Error print drivers; sire of array", cbNeeded / sizeof(LPVOID));
		MessageBox(hWnd, buffer, "Error", NULL);
	}
	delete buffer;
}

