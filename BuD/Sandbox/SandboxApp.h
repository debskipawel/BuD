#pragma once

#include <map>

#include "Core/Window.h"
#include "Core/ClientApp.h"

#include "Event/WindowEvents.h"
#include "Event/MouseEvents.h"
#include "Event/KeyboardEvents.h"

class SandboxApp : public BuD::ClientApp
{
public:
	SandboxApp();

	virtual void OnUpdate() override;

	virtual std::shared_ptr<BuD::Window> GetWindow() override;

	virtual void OnConcreteEvent(BuD::WindowResizedEvent& e) override;

	virtual void OnConcreteEvent(BuD::MouseButtonDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) override;

	virtual void OnConcreteEvent(BuD::KeyDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::KeyReleaseEvent& e) override;

private:

	void ProcessMovement();

	std::map<BuD::KeyboardKeys, bool> m_keyMap;
	std::shared_ptr<BuD::Window> m_window;

	bool m_isMoving = false;
};

std::shared_ptr<BuD::ClientApp> BuD::CreateClientApp()
{
	return std::make_shared<SandboxApp>();
}
