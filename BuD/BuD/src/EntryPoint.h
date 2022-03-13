#include "Core/Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	auto app = BuD::Application::Get();
	return app->Run(hInstance);
}
