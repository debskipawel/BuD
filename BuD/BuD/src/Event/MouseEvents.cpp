#include "MouseEvents.h"
#include "IEventDispatchable.h"

namespace BuD
{
	OVERRIDE_EVENT(MouseButtonDownEvent);
	OVERRIDE_EVENT(MouseButtonReleasedEvent);
	OVERRIDE_EVENT(MouseMovedEvent);
	OVERRIDE_EVENT(MouseScrolledEvent);
	OVERRIDE_EVENT(MouseRawInputEvent);
	OVERRIDE_EVENT(MouseInitializedEvent);
}
