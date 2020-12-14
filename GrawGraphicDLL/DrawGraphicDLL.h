#pragma once
#include "stdafx.h"
#include <atlimage.h>
#include <string>
#include "wingdi.h"

#ifdef GRAPHIC_EXPORTS
#define GRAPHIC_API __declspec(dllexport)
#else
#define GRAPHIC_API __declspec(dllimport)
#endif

extern "C" GRAPHIC_API void drawGraphic(
	HDC hdc, int x1, int y1, int x2, int y2, int p1, int p2, int width, COLORREF back, COLORREF graph);



