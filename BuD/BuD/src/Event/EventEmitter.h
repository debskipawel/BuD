#pragma once

#include "Event.h"

namespace BuD
{
	class EventEmitter
	{
	public:
		static void Emit(Event& e);
	
	private:
		EventEmitter() = default;
		virtual ~EventEmitter() = default;
	};
}
