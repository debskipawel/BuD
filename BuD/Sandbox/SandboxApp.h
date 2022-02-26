#pragma once

#include "Core/Window.h"
#include "Core/ClientApp.h"

#include "Event/WindowEvents.h"

class SandboxApp : public BuD::ClientApp
{
public:
	SandboxApp();

	virtual void OnUpdate() override;

	virtual std::shared_ptr<BuD::Window> GetWindow() override;

	virtual void OnConcreteEvent(BuD::WindowResizedEvent& e) override;

private:
	std::shared_ptr<BuD::Window> m_window;
};

std::shared_ptr<BuD::ClientApp> BuD::CreateClientApp()
{
	return std::make_shared<SandboxApp>();
}
