#include "hudzy.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nCmdShow)
{
	// Find center of screen
	INT X = GetSystemMetrics(SM_CXFULLSCREEN)/2 - (15+40+15+75+15+40+15)/2;
	INT Y = GetSystemMetrics(SM_CYFULLSCREEN)/2 - 180/2;

	// Our group holds all window regions
	HUDZYGROUP myGroup;

	// Left window
	HUDZYREGION left1;
	HUDZYREGION window1;
	HUDZYREGION right1;
	// Tubes
	HUDZYREGION tubes;
	// Right window
	HUDZYREGION left2;
	HUDZYREGION window2;
	HUDZYREGION right2;

	myGroup = CreateHudzyGroup(
		"Example1",      // group name (applies to window titles and class name)
		hThisInstance,   // instance
		NULL,            // application icon
		0x00000000,      // black - the window background color
		TRUE,            // should this window be topmost
		RGB(255,00,255), // pink - the transparency color key for images
		WndProc);        // our windows callback procedure
	if (!myGroup) return 1;

	// Left edge of window1
	left1 = CreateHudzyRegion(
		HUDZYOPT_TRANSPARENT, // this region has transparency
		X, Y, 15, 180,        // x, y, width, height
		myGroup,              // our hudzy group
		"BMPLEFT");           // resource identifier to our bmp image
	if (!left1) return 1;
	// Left window
	window1 = CreateHudzyRegion(
		HUDZYOPT_SHOW_ON_TASKBAR, // this region is what shows up when you hover on the taskbar
		X + 15 - 2, Y, 60+3 + 4, 180,
		myGroup, NULL);
	if (!window1) return 1;
	// Right edge of window1
	right1 = CreateHudzyRegion(
		HUDZYOPT_TRANSPARENT,
		X + 15 + 60, Y, 15, 180,
		myGroup,
		"BMPRIGHT");
	if (!right1) return 1;

	// Tubes
	tubes = CreateHudzyRegion(
		HUDZYOPT_TRANSPARENT,
		X + 15 + 60 + 15, Y + 70, 75, 40,
		myGroup,
		"BMPTUBES");
	if (!tubes) return 1;

	// Left edge of window2
	left2 = CreateHudzyRegion(
		HUDZYOPT_TRANSPARENT,
		X + 15 + 60 + 15 + 75, Y, 15, 180,
		myGroup,
		"BMPLEFT");
	if (!left2) return 1;
	// Right window
	window2 = CreateHudzyRegion(
		0,
		X -2 + 15 + 60 + 15 + 75 + 15, Y, 60 + 4, 180,
		myGroup, NULL);
	if (!window2) return 1;
	// Right edge of window2
	right2 = CreateHudzyRegion(
		HUDZYOPT_TRANSPARENT,
		X + 15 + 60 + 15 + 75 + 15 + 60, Y, 15, 180,
		myGroup,
		"BMPRIGHT");
	if (!right2) return 1;

	// Put buttons into left window
	HWND hwndLeft = HudzyGetHWND(window1);
	for (int i = 0; i < 6; i++)
		CreateWindow("BUTTON", "Button", BS_PUSHBUTTON | BS_VCENTER | WS_CHILD | WS_VISIBLE, 2, 12 + i*26, 60, 20, hwndLeft, NULL, hThisInstance, 0);
		
	// Put listbox into right window
	HWND hwndRight = HudzyGetHWND(window2);
	HWND list = CreateWindow("LISTBOX", "", WS_CHILD | LBS_STANDARD | LBS_NOINTEGRALHEIGHT | WS_VISIBLE, 2, 12, 60, 180-24, hwndRight, NULL, hThisInstance, NULL);
	for (int i = 0; i < 6; i++)
		SendMessage(list, LB_ADDSTRING, 0, (LPARAM)"item");

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		HudzyDraw(myGroup);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}