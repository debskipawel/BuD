#pragma once

#include <map>
#include <memory>

#include <BuD.h>

#include "Gui/SceneEditor.h"

class SandboxApp : public BuD::ClientApp
{
public:
	SandboxApp(const BuD::DX11Device& device);

	inline virtual std::shared_ptr<BuD::AbstractCamera> GetCamera() override { return m_camera; }
	virtual const std::vector<std::shared_ptr<BuD::Mesh>>& GetModels() override;

	virtual void OnUpdate() override;
	virtual void OnGuiRender() override;

	virtual void OnConcreteEvent(BuD::MouseButtonDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseMovedEvent& e) override;

	virtual void OnConcreteEvent(BuD::KeyDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::KeyReleaseEvent& e) override;

	virtual void OnConcreteEvent(BuD::WindowResizedEvent& e) override;

private:

	void ProcessMovement();

	std::unique_ptr<SceneEditor> m_gui;

	std::map<BuD::KeyboardKeys, bool> m_keyMap;
	std::shared_ptr<BuD::AbstractCamera> m_camera;

	std::shared_ptr<BuD::Torus> m_torus;
	std::vector<BuD::Point> m_points;

	std::vector<std::shared_ptr<BuD::Mesh>> m_models;

	bool m_isMoving = false;
};

std::shared_ptr<BuD::ClientApp> BuD::CreateClientApp(const BuD::DX11Device& device)
{
	return std::make_shared<SandboxApp>(device);
}
