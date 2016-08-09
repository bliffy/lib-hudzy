// NOTICE: Do not include this file in your project! This file is an internal portion of the Hudzy library.
/*
Copyright (c) 2016 - Michael E Jolley

This library is free software: you can redistribute it and/or modify
it under the terms of the MIT License.
You should have received a copy of the MIT License along with this program.
*/

#ifndef __HUDZY_LIB_INTERNAL_H__
#define __HUDZY_LIB_INTERNAL_H__

namespace _HUDZY_INTERNAL {

	VOID HudzySetLastError(HUDZYERROR dwError);
	LRESULT CALLBACK InternalWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	class HudzyRegion;

	class HudzyGroup {
	public:
		HudzyGroup(LPCSTR lpName, HINSTANCE hInstance, HICON hIcon, COLORREF crBgColor, BOOL bAlwaysForeground, COLORREF crTransparencyColor, WNDPROC lpfnWndProc);
		~HudzyGroup(VOID);
		inline VOID addRegion(HudzyRegion* region) { regions.push_back(region); }
		VOID drag(BOOL reset);
		VOID draw(VOID);
		VOID bringToFront(HWND hwndExclude);

		WNDCLASSEX  wc;
		WNDPROC outerWndProc;
		BOOL foreground;
		COLORREF transparency;
		std::vector<HudzyRegion*> regions;
	};

	class HudzyRegion {
	public:
		HudzyRegion(DWORD dwOpts, UINT iX, UINT iY, UINT iW, UINT iH, HudzyGroup* hGroup, LPCSTR lpImage);
		~HudzyRegion(VOID);
		void draw(COLORREF crTransColor);
		VOID moveBy(INT xDiff, INT yDiff);
		inline VOID setBgSrcOffset(UINT x, UINT y) { bmOffset.x = x; bmOffset.y = y; }

		POINT    pos;
		SIZE     size;
		BOOL     transparent;
		HBITMAP  hbm;
		POINT    bmOffset;
		HWND     hwnd;
		BOOL     needsMove;
	};

} // namespace

#endif