#include "GuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

namespace BuD
{
	GuiLayer::GuiLayer(std::shared_ptr<DX11Renderer> renderer, std::shared_ptr<Win32Window> window)
		: m_renderer(renderer)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(window->Handle());
		ImGui_ImplDX11_Init(renderer->Device().Raw(), renderer->Device().Context().Get());
	}

	GuiLayer::~GuiLayer()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void GuiLayer::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	
	void GuiLayer::EndFrame()
	{
		m_renderer->DisableDepthBuffer();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
