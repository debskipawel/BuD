#pragma once

#include <memory>

#include "Window.h"
#include "Renderer.h"

#include "Camera/AbstractCamera.h"
#include "DirectX11/DX11Device.h"
#include "Event/IEventDispatchable.h"
#include "Scene/RenderableSceneEntity.h"

namespace BuD
{
	class ClientApp : public IEventDispatchable
	{
	public:
		virtual void OnUpdate() = 0;
		virtual std::shared_ptr<AbstractCamera> GetCamera() = 0;
		virtual const std::vector<std::shared_ptr<RenderableSceneEntity>>& GetModels() = 0;

	protected:
		ClientApp() = default;
		virtual ~ClientApp() = default;
	};

	extern std::shared_ptr<ClientApp> CreateClientApp(const DX11Device& device);
}
