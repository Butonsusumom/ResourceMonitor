#include "stdafx.h"
#include "Base.h"

#define INFO_BUFFER_SIZE 32767

void PrintINF_PC(HWND hWnd, int nameedit, int compEdit) // Get System Info
{
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	//User Name
	bufCharCount = INFO_BUFFER_SIZE;
	if (GetUserName(infoBuf, &bufCharCount))
	{
		SendMessage(GetDlgItem(hWnd, nameedit), WM_SETTEXT, NULL, (LPARAM)infoBuf);
	}
	//Computer Name
	bufCharCount = INFO_BUFFER_SIZE;
	if (GetComputerName(infoBuf, &bufCharCount))
	{
		SendMessage(GetDlgItem(hWnd, compEdit), WM_SETTEXT, NULL, (LPARAM)infoBuf);
	}

}