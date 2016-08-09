/*
Copyright (c) 2016 - Michael E Jolley

This library is free software: you can redistribute it and/or modify
it under the terms of the MIT License.
You should have received a copy of the MIT License along with this program.
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

	HWND HudzyGetHWND(HUDZYREGION hRegion);

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

	VOID HudzyReleaseGroup(HUDZYGROUP& group);

#ifdef __cplusplus
}
#endif
#endif