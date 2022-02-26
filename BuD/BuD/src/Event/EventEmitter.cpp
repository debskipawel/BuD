#include "EventEmitter.h"

#include "../Core/Application.h"

namespace BuD
{
	void EventEmitter::Emit(Event& e)
	{
		auto app = Application::Get();
		_ASSERT(app);

		app->OnEvent(e);
	}
}