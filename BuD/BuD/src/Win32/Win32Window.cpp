#ifdef _WIN32

#include "Win32Window.h"
#include "Win32EventFactory.h"

#include "../DirectX11/DX11Renderer.h"

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
		WNDCLASSEXW wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = nullptr;
		wcex.hIcon = LoadIconW(nullptr, L"IDI_ICON");
		wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wcex.lpszClassName = reinterpret_cast<LPCWSTR>(appInfo.name.c_str());
		wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");

		ATOM wClass = RegisterClassExW(&wcex);
		if (!wClass)
		{
			fprintf(stderr, "%s\n", "Couldn’t create Window Class");
			return;
		}

		m_hwnd = CreateWindowExW(
			0, reinterpret_cast<LPCWSTR>(appInfo.name.c_str()), reinterpret_cast<LPCWSTR>(appInfo.windowTitle.c_str()),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
			m_width, m_height, nullptr, nullptr, GetModuleHandle(NULL), nullptr
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
