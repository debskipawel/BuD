#ifdef _WIN32

#include "Win32Window.h"

#include <Windows.h>
#include <stdio.h>

namespace BuD
{
	void PaintWindow(HWND hwnd)
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		SetPixel(hdc, 10, 10, RGB(0, 0xFF, 0));  // green
		EndPaint(hwnd, &ps);
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_PAINT:
		{
			PaintWindow(hwnd);
			return 0;
		}
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	Win32Window::Win32Window(const ApplicationInfo& appInfo)
		: m_msg(), m_hwnd(NULL)
	{
		// Register window class
		WNDCLASSA wc =
		{
			0, WndProc, 0, 0, 0,
			LoadIcon(NULL, IDI_APPLICATION),
			LoadCursor(NULL, IDC_ARROW),
			reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)), // background color == black
			NULL, // no menu
			"ExampleWindowClass"
		};

		ATOM wClass = RegisterClassA(&wc);
		if (!wClass)
		{
			fprintf(stderr, "%s\n", "Couldn’t create Window Class");
			return;
		}

		// Create the window
		m_hwnd = CreateWindowW(
			MAKEINTATOM(wClass),
			appInfo.windowTitle.c_str(),     // window title
			WS_OVERLAPPEDWINDOW, // title bar, thick borders, etc.
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			m_width,
			m_height,
			NULL, // no parent window
			NULL, // no menu
			GetModuleHandle(NULL),  // EXE's HINSTANCE
			NULL  // no magic user data
		);

		if (!m_hwnd)
		{
			fprintf(stderr, "%ld\n", GetLastError());
			fprintf(stderr, "%s\n", "Failed to create Window");
			return;
		}
	}
	void Win32Window::Show()
	{
		ShowWindow(m_hwnd, SW_SHOWNORMAL);
	}

	bool Win32Window::EventQueueNotEmpty()
	{
		if (!m_messageRead)
		{
			m_messageRead = GetMessage(&m_msg, NULL, 0, 0) > 0;
		}

		return m_messageRead;
	}

	void Win32Window::HandleEvent()
	{
		if (EventQueueNotEmpty())
		{
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);

			m_messageRead = false;
		}
	}
}

#endif
