#ifdef _WIN32

#include "Win32Window.h"
#include "Win32EventFactory.h"

#include "DirectX11/DX11Renderer.h"

#include "Event/EventEmitter.h"

#include <Windows.h>
#include <stdio.h>

#include "imgui.h"
#include "backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace BuD
{
	constexpr UINT baseWidth = 1200;
	constexpr UINT baseHeight = 800;

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return true;

		if (ImGui::GetCurrentContext() && ImGui::GetIO().WantCaptureMouse)
			return true;

		if (auto e = Win32EventFactory::Get().Construct(hwnd, msg, wParam, lParam))
		{
			EventEmitter::Emit(*e);
			return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	Win32Window::Win32Window(const ApplicationInfo& appInfo, HINSTANCE hInstance)
		: m_hwnd(NULL), m_hInstance(hInstance)
	{
		WNDCLASSEXW wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = hInstance;
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

		RECT rect = { 0, 0, baseWidth, baseHeight };
		DWORD style = WS_OVERLAPPEDWINDOW;

		m_hwnd = CreateWindowExW(
			0, reinterpret_cast<LPCWSTR>(appInfo.name.c_str()), reinterpret_cast<LPCWSTR>(appInfo.windowTitle.c_str()),
			style, CW_USEDEFAULT, CW_USEDEFAULT, 
			rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr
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
	
	int Win32Window::Width()
	{
		RECT r;
		GetClientRect(m_hwnd, &r);

		return r.right - r.left;
	}

	int Win32Window::Height()
	{
		RECT r;
		GetClientRect(m_hwnd, &r);

		return r.bottom - r.top;
	}
	
	void Win32Window::ToggleFullscreen()
	{
		if (m_fullscreen)
		{
			SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, 0);

			ShowWindow(m_hwnd, SW_SHOWNORMAL);

			SetWindowPos(m_hwnd, HWND_TOP, 0, 0, baseWidth, baseHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}
		else
		{
			SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_POPUP);
			SetWindowLongPtr(m_hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

			SetWindowPos(m_hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			ShowWindow(m_hwnd, SW_SHOWMAXIMIZED);
		}

		m_fullscreen = !m_fullscreen;
	}
}

#endif
