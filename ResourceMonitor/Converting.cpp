#include "stdafx.h"
#include "Base.h"
#pragma warning(disable : 4996)

	wchar_t* buf;
wchar_t * ItoT(int i)
{
	if (!buf) free(buf);
	buf = (wchar_t*)calloc(10, sizeof(wchar_t));
	wsprintf(LPSTR(buf), "%d", i);
	return buf;
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
