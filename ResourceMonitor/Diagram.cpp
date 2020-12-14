#include "stdafx.h"
#include "Base.h"
#include <TlHelp32.h>

HINSTANCE h = LoadLibrary("C:/Users/Ksusha/Source/Repos/String_Replace_DLL/Debug/Dll.dll");

void DrawDiagram(HWND hWnd, int x1, int y1, int x2, int y2, int p1, int p2, int width, COLORREF back, COLORREF graph)
{
	void(*DllFunc) (HWND hWnd, int x1, int y1, int x2, int y2, int p1, int p2, int width, COLORREF back, COLORREF graph);
	DllFunc = (void(*) (HWND hWnd, int x1, int y1, int x2, int y2, int p1, int p2, int width, COLORREF back, COLORREF graph))
	GetProcAddress(h, "drawGraphic");
	DllFunc(hWnd, x1, y1, x2, y2, p1, p2, width, back, graph);
}

