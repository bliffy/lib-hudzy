// NOTICE: Do not include this file in your project! This file is an internal portion of the Hudzy library.
/*
The MIT License

Copyright (c) 2016 Michael E Jolley

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
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