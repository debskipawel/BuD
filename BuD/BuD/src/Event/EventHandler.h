#pragma once

#include <functional>

#include "Event.h"

namespace BuD
{
	using EventHandler = std::function<void(Event&)>;

	struct EventHandlerInfo
	{
		EventHandler m_handler;
	};
}
