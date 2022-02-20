#include "Core/Application.h"

int main()
{
	auto app = BuD::Application::Get();
	return app->Run();
}