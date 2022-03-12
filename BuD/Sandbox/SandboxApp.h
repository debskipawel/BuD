#pragma once

#include <map>
#include <memory>

#include <BuD.h>

class SandboxApp : public BuD::ClientApp
{
public:
	SandboxApp();

	virtual void OnUpdate() override;

	virtual void OnConcreteEvent(BuD::MouseButtonDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) override;

	virtual void OnConcreteEvent(BuD::KeyDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::KeyReleaseEvent& e) override;

private:

	void ProcessMovement();

	std::map<BuD::KeyboardKeys, bool> m_keyMap;

	bool m_isMoving = false;
};

std::shared_ptr<BuD::ClientApp> BuD::CreateClientApp()
{
	return std::make_shared<SandboxApp>();
}
