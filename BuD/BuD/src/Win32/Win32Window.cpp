#ifdef _WIN32

#include "Win32Window.h"
#include "Win32EventFactory.h"

#include "DirectX11/DX11Renderer.h"

#include "../Event/EventEmitter.h"

#include <Windows.h>
#include <stdio.h>

namespace BuD
{
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (auto e = Win32EventFactory::Get().Construct(hwnd, msg, wParam, lParam))
		{
			EventEmitter::Emit(*e);
			return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	Win32Window::Win32Window(const ApplicationInfo& appInfo)
		: m_hwnd(NULL)
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

		m_renderer = std::make_shared<DX11Renderer>(m_hwnd, m_width, m_height);
	}
	void Win32Window::Show()
	{
		ShowWindow(m_hwnd, SW_SHOWNORMAL);
	}

	void Win32Window::ProcessEvents()
	{
		MSG msg = {};

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

#endif
