#include "stdafx.h"
#include "Base.h"

void DrawGraph(HWND hWnd, int percent, int X1, int Y1, int X2, int Y2) //graw percent
{
	HDC hdc = GetDC(hWnd); // Get DC
	HBRUSH hbrush;

	RECT Rec_1 = { X1, Y1, X2, Y2 };
	hbrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &Rec_1, hbrush);

	RECT Rec_2 = { X1 + 1, Y2 - (Y2 - Y1)* percent / 100 , X2 - 1, Y2 - 1 };
	hbrush = CreateSolidBrush(RGB(202, 167, 166));
	FillRect(hdc, &Rec_2, hbrush);

	ReleaseDC(hWnd, hdc);
	DeleteDC(hdc);
}

void DrawRect(HWND hWnd, int x1, int y1, int x2, int y2, COLORREF back)
{
	HDC hdc = GetDC(hWnd); // Get DC
	RECT Rec = { x1, y1, x2, y2 };
	HBRUSH hbrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &Rec, hbrush);
	ReleaseDC(hWnd, hdc);
	DeleteDC(hdc);
	DeleteObject(SelectObject(hdc, hbrush));
}