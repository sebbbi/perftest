#include "window.hpp"
#include <assert.h>
//#include <memory>

LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND createWindow(uint2 resolution/*, WNDPROC windowProc*/)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = windowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "perftest";
	auto result = RegisterClassEx(&wc);
	assert(result > 0);

	//DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD style = WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = resolution.x;
	rect.bottom = resolution.y;
	BOOL success = AdjustWindowRectEx(&rect, style, false, 0);
	assert(success);

	HWND window = CreateWindowEx(NULL,
									"perftest", // class name
									"perftest", // title
									style,
									CW_USEDEFAULT, // x
									CW_USEDEFAULT, // y
									rect.right - rect.left, // width
									rect.bottom - rect.top, // height
									nullptr, // parent
									nullptr, // menu
									GetModuleHandle(0),
									nullptr); // extra data

	assert(window);

	ShowWindow(window, SW_SHOWDEFAULT);

	//SetCapture(window);

	return window;
}

MessageStatus messagePump()
{
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_QUIT)
			return MessageStatus::Exit;
	}

	return MessageStatus::Default;
}

