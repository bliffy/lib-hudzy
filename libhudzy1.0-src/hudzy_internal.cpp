// NOTICE: Do not include this file in your project! This file is an internal portion of the Hudzy library.
/*
Copyright (c) 2016 - Michael E Jolley

This library is free software: you can redistribute it and/or modify
it under the terms of the MIT License.
You should have received a copy of the MIT License along with this program.
*/

#include <vector>
#include <cassert>
#include "hudzy.h"
#include "hudzy_internal.h"

namespace _HUDZY_INTERNAL {

	// Soft error state.
	HUDZYERROR dwLastError = HUDZYERROR_OK;
	VOID HudzySetLastError(HUDZYERROR dwError) { dwLastError = dwError; }


	// Group methods.

	HudzyGroup::HudzyGroup(LPCSTR lpName, HINSTANCE hInstance, HICON hIcon, COLORREF crBgColor, BOOL bAlwaysForeground, COLORREF crTransparencyColor, WNDPROC lpfnWndProc)
		: outerWndProc(lpfnWndProc), foreground(bAlwaysForeground), transparency(crTransparencyColor)
	{
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.hInstance = hInstance;
		wc.lpszClassName = lpName;
		wc.lpfnWndProc = InternalWndProc;
		wc.style = CS_DBLCLKS;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.hIcon = hIcon;
		wc.hIconSm = hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = CreateSolidBrush(crBgColor);
#ifdef _WIN64
		assert(sizeof(LONG) != sizeof(LONG*));
		wc.cbWndExtra = 2 * sizeof(LONG*);
#else
		assert(sizeof(LONG) == sizeof(LONG*));
		wc.cbWndExtra = 2 * sizeof(LONG);
#endif
		if (!RegisterClassEx(&wc))
			HudzySetLastError(HUDZYERROR_CLASS_REGISTRY);
	}

	HudzyGroup::~HudzyGroup(VOID)
	{
		for (std::vector<HudzyRegion*>::iterator i = regions.begin(); i != regions.end(); i++)
			if (*i != NULL) delete (*i);
		UnregisterClass(wc.lpszClassName, wc.hInstance);
	}

	VOID HudzyGroup::drag(BOOL reset)
	{
		static POINT last;
		POINT current;
		GetCursorPos(&current);
		if (!reset)
		{
			register INT xDiff = (current.x - last.x);
			register INT yDiff = (current.y - last.y);
			for (std::vector<HudzyRegion*>::iterator i = regions.begin(); i != regions.end(); i++)
			{
				(*i)->moveBy(xDiff, yDiff);
			}
		}
		last.x = current.x; last.y = current.y;
	}

	VOID HudzyGroup::draw(VOID)
	{
		for (std::vector<HudzyRegion*>::iterator i = regions.begin(); i != regions.end(); i++)
			(*i)->draw(transparency);
	}

	VOID HudzyGroup::bringToFront(HWND hwndExclude)
	{
		for (std::vector<HudzyRegion*>::iterator i = regions.begin(); i != regions.end(); i++)
			if (hwndExclude != (*i)->hwnd) SetForegroundWindow((*i)->hwnd);
	}


	// Region methods.

	HudzyRegion::HudzyRegion(DWORD dwOpts, UINT iX, UINT iY, UINT iW, UINT iH, HudzyGroup* hGroup, LPCSTR lpImage)
		: hbm(NULL), hwnd(NULL), needsMove(FALSE)
	{
		pos.x = iX;
		pos.y = iY;
		bmOffset.x = 0;
		bmOffset.y = 0;
		size.cx = iW;
		size.cy = iH;

		if (lpImage != 0)
		{
			hbm = LoadBitmap(((HudzyGroup*)hGroup)->wc.hInstance, lpImage);
			if (hbm == NULL)
			{
				HudzySetLastError(HUDZYERROR_IMAGE_LOAD);
				return;
			}
		}

		transparent = (dwOpts & HUDZYOPT_TRANSPARENT);
		DWORD flags = 0;
		flags |= (transparent ? WS_EX_LAYERED : 0);
		flags |= (dwOpts & HUDZYOPT_SHOW_ON_TASKBAR ? 0 : WS_EX_TOOLWINDOW);
		flags |= (((HudzyGroup*)hGroup)->foreground ? WS_EX_TOPMOST : 0);
		hwnd = CreateWindowEx(
			flags,
			((HudzyGroup*)hGroup)->wc.lpszClassName,
			((HudzyGroup*)hGroup)->wc.lpszClassName,
			WS_POPUP | WS_VISIBLE,
			iX, iY, iW, iH,
			HWND_DESKTOP,
			NULL,
			((HudzyGroup*)hGroup)->wc.hInstance,
			NULL);
		if (hwnd != NULL)
		{
#ifdef _WIN64
			// Store user callback in custom hwnd storage space 0
			SetWindowLongPtr(hwnd, 0, (LONG_PTR)((HudzyGroup*)hGroup)->outerWndProc);
			// Store group pointer in space 1
			SetWindowLongPtr(hwnd, sizeof(LONG_PTR), (LONG_PTR)hGroup);
#else
			// Store user callback in custom hwnd storage space 0
			SetWindowLong(hwnd, 0, (LONG)(((HudzyGroup*)hGroup)->outerWndProc));
			// Store group pointer in space 1
			SetWindowLong(hwnd, sizeof(LONG), (LONG)hGroup);
#endif
		}
		else
			HudzySetLastError(HUDZYERROR_WINDOW_CREATE);
	}

	HudzyRegion::~HudzyRegion(VOID)
	{
		if (hwnd != NULL) CloseWindow(hwnd);
		if (hbm != NULL) DeleteObject(hbm);
	}

	void HudzyRegion::draw(COLORREF crTransColor)
	{
		if (needsMove)  // implies !transparent
		{
			SetWindowPos(hwnd, NULL, pos.x, pos.y, 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOSENDCHANGING | SWP_NOSIZE | SWP_NOZORDER);
			needsMove = FALSE;
		}
		if (hbm != NULL)
		{
			HDC hRealDC = GetDC(hwnd);
			HDC hdcMemory = CreateCompatibleDC(hRealDC);
			SelectObject(hdcMemory, hbm); // the bitmap

			if (transparent)
			{
				BLENDFUNCTION bfunc;
				ZeroMemory(&bfunc, sizeof(BLENDFUNCTION));
				bfunc.BlendOp = AC_SRC_OVER;
				UpdateLayeredWindow(
					hwnd,
					hRealDC,
					&pos,
					&size,
					hdcMemory,
					&bmOffset,
					crTransColor,
					&bfunc,
					ULW_COLORKEY);
			}
			else
				BitBlt(hRealDC, 0, 0, size.cx, size.cy, hdcMemory, bmOffset.x, bmOffset.y, SRCCOPY);
			ReleaseDC(hwnd, hRealDC);
			DeleteDC(hdcMemory);
		}
	}

	VOID HudzyRegion::moveBy(INT xDiff, INT yDiff)
	{
		if (xDiff == 0 && yDiff == 0)
			return;
		pos.x += xDiff; pos.y += yDiff;
		if (!transparent)
			needsMove = TRUE;
	}


	// Internal callback.

	LRESULT CALLBACK InternalWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static bool bDragging = false;
#ifdef _WIN64
		WNDPROC userProc = (WNDPROC)GetWindowLongPtr(hwnd, 0);
		HudzyGroup* group = (HudzyGroup*)GetWindowLongPtr(hwnd, sizeof(LONG_PTR));
#else
		WNDPROC userProc = (WNDPROC)GetWindowLong(hwnd, 0);
		HudzyGroup* group = (HudzyGroup*)GetWindowLong(hwnd, sizeof(LONG));
#endif
		switch (message)
		{
		case WM_LBUTTONDOWN:
			group->drag(bDragging = true);
			SetCapture(hwnd);
			break;
		case WM_LBUTTONUP:
			if (bDragging)
			{
				bDragging = false;
				ReleaseCapture();
			}
			break;
		case WM_MOUSEMOVE:
			if (bDragging)
				group->drag(false);
			break;
		case WM_ACTIVATE:
			if (LOWORD(wParam) != 0 && group != NULL) // do not remove null-check!
			{
				static bool noonce = false;
				if (!noonce) // prevents recursive calls to bringToFront() by each window
				{
					noonce = true;
					group->bringToFront(NULL);
					noonce = false;
				}
			}
			break;
		default:
			break;
		}

		if (userProc != NULL) return CallWindowProc(userProc, hwnd, message, wParam, lParam);
		else return DefWindowProc(hwnd, message, wParam, lParam);
	}
}


// API functions.

HUDZYGROUP CreateHudzyGroup(LPCSTR lpGroupName, HINSTANCE hInstance, HICON hIcon, COLORREF crBgColor, BOOL bAlwaysForeground, COLORREF crTransparencyColor, WNDPROC lpfnWndProc)
{
	assert((lpGroupName != NULL && hInstance != NULL));
	_HUDZY_INTERNAL::HudzyGroup* g = new _HUDZY_INTERNAL::HudzyGroup(
		lpGroupName,
		hInstance,
		hIcon,
		crBgColor,
		bAlwaysForeground,
		crTransparencyColor,
		lpfnWndProc);
	if (_HUDZY_INTERNAL::dwLastError != HUDZYERROR_OK)
	{
		delete g;
		return NULL;
	}
	return g;
}

HUDZYREGION CreateHudzyRegion(DWORD dwOptFlags, UINT iXPos, UINT iYPos, UINT iWidth, UINT iHeight, HUDZYGROUP hGroup, LPCSTR lpBgImage)
{
	assert(hGroup != NULL);
	_HUDZY_INTERNAL::HudzyRegion* r = new _HUDZY_INTERNAL::HudzyRegion(
		dwOptFlags,
		iXPos,
		iYPos,
		iWidth,
		iHeight,
		(_HUDZY_INTERNAL::HudzyGroup*)hGroup,
		lpBgImage);
	if (_HUDZY_INTERNAL::dwLastError != HUDZYERROR_OK)
	{
		delete r;
		return NULL;
	}
	((_HUDZY_INTERNAL::HudzyGroup*)hGroup)->addRegion(r);
	return r;
}

VOID UpdateBackgroundSourceOffset(HUDZYREGION hRegion, UINT iXOffset, UINT iYOffset)
{
	assert(hRegion != NULL);
	((_HUDZY_INTERNAL::HudzyRegion*)hRegion)->setBgSrcOffset(iXOffset, iYOffset);
}

VOID HudzyDraw(HUDZYGROUP hGroup)
{
	assert(hGroup != NULL);
	((_HUDZY_INTERNAL::HudzyGroup*)hGroup)->draw();
}

HWND HudzyGetHWND(HUDZYREGION hRegion)
{
	assert(hRegion != NULL);
	return ((_HUDZY_INTERNAL::HudzyRegion*)hRegion)->hwnd;
}

HUDZYERROR HudzyGetLastError(VOID)
{
	return _HUDZY_INTERNAL::dwLastError;
}

VOID HudzyReleaseGroup(HUDZYGROUP& hGroup)
{
	if (hGroup != NULL) delete (_HUDZY_INTERNAL::HudzyGroup*)hGroup;
	hGroup = NULL;
}

