#pragma once

#include <memory>

#include "Window.h"

#include "../Event/IEventDispatchable.h"

namespace BuD
{
	class ClientApp : public IEventDispatchable
	{
	public:
		virtual void OnUpdate() = 0;

		virtual std::shared_ptr<Window> GetWindow() = 0;

	protected:
		ClientApp() = default;
		virtual ~ClientApp() = default;
	};

	extern std::shared_ptr<ClientApp> CreateClientApp();
}
