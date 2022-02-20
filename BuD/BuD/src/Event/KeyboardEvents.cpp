#include "KeyboardEvents.h"
#include "IEventDispatchable.h"

namespace BuD
{
	OVERRIDE_EVENT(KeyDownEvent);
	OVERRIDE_EVENT(KeyReleaseEvent);
	OVERRIDE_EVENT(KeyRepeatEvent);
}
