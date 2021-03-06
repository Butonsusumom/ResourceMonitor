// GrawGraphicDLL.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include <atlimage.h>
#include <string>
#include "wingdi.h"
#include "windef.h"
#include "DrawGraphicDLL.h"

BITMAPINFOHEADER createBitmapHeader(int width, int height);
HBITMAP ScreenCapture(HDC hwindowDC, int screenx, int screeny, int width, int height);
void PutBitmap(HDC screen, HBITMAP bmp, int X, int Y, int W, int H);
void draw(HDC hdc, int x1, int y1, int x2, int y2, int p1, int p2, COLORREF color);
void DrawRect(HDC hdc, int x1, int y1, int x2, int y2, COLORREF back);

void drawGraphic(HWND hWnd, int x1, int y1, int x2, int y2, int p1, int p2, int width, COLORREF back, COLORREF graph, COLORREF fill)
{
	HDC hdc = GetDC(hWnd);
	HBITMAP bmp = ScreenCapture(hdc, x1 + (x2 - x1) / 60, y1, (x2 - x1) * 59 / 60, y2 - y1);
	PutBitmap(hdc, bmp, x1, y1, x2 - (x2 - x1) / 60, y2);

	DrawRect(hdc, x1 + (x2 - x1) * 59 / 60, y1, x2, y2, back);

	HPEN hPen = CreatePen(PS_SOLID, width, graph);
	SelectObject(hdc, hPen);

	draw(hdc, x1, y1, x2, y2, p1, p2,fill);
	MoveToEx(hdc, x2 - (x2 - x1) / 60, y2 - (y2 - y1)*p1 / 100, NULL);
	LineTo(hdc, x2, y2 - (y2 - y1)*p2 / 100);
	DeleteDC(hdc);
	DeleteObject(bmp);
	DeleteObject(SelectObject(hdc, hPen));
}

void draw(HDC hdc, int x1, int y1, int x2, int y2, int p1, int p2, COLORREF color)
{
	POINT Pt[4];
	Pt[0].x = x1 + (x2 - x1) * 59 / 60;
	Pt[0].y = y2;
	Pt[1].x = x1 + (x2 - x1) * 59 / 60;
	Pt[1].y = y2 - (y2 - y1)*p1 / 100;
	Pt[2].x = x2;
	Pt[2].y = y2 - (y2 - y1)*p2 / 100;
	Pt[3].x = x2;
	Pt[3].y = y2;
	HBRUSH hBrush = CreateSolidBrush(color);
	HBRUSH old = (HBRUSH)SelectObject(hdc, hBrush);
	Polygon(hdc, Pt, 4);
	DeleteObject(SelectObject(hdc, old));
	DeleteObject(SelectObject(hdc, hBrush));
}

void DrawRect(HDC hdc, int x1, int y1, int x2, int y2, COLORREF back)
{
	RECT Rec = { x1, y1, x2, y2 };
	HBRUSH hbrush = CreateSolidBrush(back);
	FillRect(hdc, &Rec, hbrush);
}

void PutBitmap(HDC screen, HBITMAP bmp, int X, int Y, int W, int H)
{
	HDC bitmap = CreateCompatibleDC(screen);
	HBITMAP old = (HBITMAP)SelectObject(bitmap, bmp);
	BitBlt(screen, X, Y, W, H, bitmap, 0, 0, SRCCOPY);
	SelectObject(bitmap, old);
	DeleteDC(bitmap);
	DeleteObject(old);
	//ReleaseDC(NULL, screen);
}

HBITMAP ScreenCapture(HDC hwindowDC, int screenx, int screeny, int width, int height)
{
	// get handles to a device context (DC)
	HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	// define scale
	int scale = 1;

	// create a bitmap
	HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	BITMAPINFOHEADER bi = createBitmapHeader(width, height);

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);

	// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that call HeapAlloc using a handle to the process's default heap.
	// Therefore, GlobalAlloc and LocalAlloc have greater overhead than HeapAlloc.
	DWORD dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	char* lpbitmap = (char*)GlobalLock(hDIB);

	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);   //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	// avoid memory leak
	DeleteDC(hwindowCompatibleDC);

	return hbwindow;
}

BITMAPINFOHEADER createBitmapHeader(int width, int height)
{
	BITMAPINFOHEADER  bi;

	// create a bitmap
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	return bi;
}

