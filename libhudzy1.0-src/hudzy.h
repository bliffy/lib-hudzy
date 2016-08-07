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

#ifndef __HUDZY_LIB_H__
#define __HUDZY_LIB_H__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef HANDLE HUDZYGROUP;
	typedef HANDLE HUDZYREGION;

	HUDZYGROUP CreateHudzyGroup(
		LPCSTR     lpGroupName,
		HINSTANCE  hInstance,
		HICON      hIcon,
		COLORREF   crBgColor,
		BOOL       bAlwaysForeground,
		COLORREF   crTransparencyColor = 0x00ff0000,
		WNDPROC    lpfnWndProc = NULL
		);

	HUDZYREGION CreateHudzyRegion(
		DWORD       dwOptFlags,
		UINT        iXPos,
		UINT        iYPos,
		UINT        iWidth,
		UINT        iHeight,
		HUDZYGROUP  hGroup,
		LPCSTR      lpBgImage = 0
		);

	VOID UpdateBackgroundSourceOffset(
		HUDZYREGION hRegion,
		UINT iXOffset,
		UINT iYOffset
		);

	VOID HudzyDraw(HUDZYGROUP hGroup);

	enum {
		HUDZYOPT_TRANSPARENT = 1,
		HUDZYOPT_SHOW_ON_TASKBAR = 2
	};

	enum HUDZYERROR {
		HUDZYERROR_OK = 0,
		HUDZYERROR_CLASS_REGISTRY,
		HUDZYERROR_IMAGE_LOAD,
		HUDZYERROR_WINDOW_CREATE
	};

	HUDZYERROR HudzyGetLastError(VOID);

#ifdef __cplusplus
}
#endif
#endif