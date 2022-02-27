#pragma once

#include <map>

#include <BuD.h>

class SandboxApp : public BuD::ClientApp
{
public:
	SandboxApp();

	virtual void OnUpdate() override;

	virtual std::shared_ptr<BuD::Window> GetWindow() override;
	virtual std::shared_ptr<BuD::Renderer> GetRenderer() override;

	virtual void OnConcreteEvent(BuD::MouseButtonDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) override;

	virtual void OnConcreteEvent(BuD::KeyDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::KeyReleaseEvent& e) override;

private:

	void ProcessMovement();

	std::map<BuD::KeyboardKeys, bool> m_keyMap;
	std::shared_ptr<BuD::Win32Window> m_window;
	std::shared_ptr<BuD::DX11Renderer> m_renderer;

	bool m_isMoving = false;
};

std::shared_ptr<BuD::ClientApp> BuD::CreateClientApp()
{
	return std::make_shared<SandboxApp>();
}
