#include "WindowEvents.h"
#include "IEventDispatchable.h"

namespace BuD
{
	OVERRIDE_EVENT(WindowMovedEvent);
	OVERRIDE_EVENT(WindowResizedEvent);
	OVERRIDE_EVENT(WindowClosedEvent);
	OVERRIDE_EVENT(WindowEnterSizeMoveEvent);
	OVERRIDE_EVENT(WindowExitSizeMoveEvent);
	OVERRIDE_EVENT(ActivateAppEvent);
	OVERRIDE_EVENT(ToggleFullscreenEvent);
}
