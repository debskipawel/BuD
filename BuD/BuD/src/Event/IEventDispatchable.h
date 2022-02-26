#pragma once

#include "Event.h"

namespace BuD
{
	class KeyDownEvent;
	class KeyReleaseEvent;
	class KeyRepeatEvent;
	class MouseButtonDownEvent;
	class MouseButtonReleasedEvent;
	class MouseMovedEvent;
	class MouseScrolledEvent;
	class MouseRawInputEvent;
	class MouseInitializedEvent;
	class WindowResizedEvent;
	class WindowMovedEvent;
	class WindowClosedEvent;
	class WindowEnterSizeMoveEvent;
	class WindowExitSizeMoveEvent;
	class ActivateAppEvent;
	class ToggleFullscreenEvent;


	class IEventDispatchable
	{
	public:
		inline virtual void OnEvent(Event& e) { e.Visit(*this); }

		virtual ~IEventDispatchable() = default;

		// ----- KEYBOARD EVENTS
		REGISTER_EVENT(KeyDownEvent);
		REGISTER_EVENT(KeyReleaseEvent);
		REGISTER_EVENT(KeyRepeatEvent);

		// ----- MOUSE EVENTS
		REGISTER_EVENT(MouseButtonDownEvent);
		REGISTER_EVENT(MouseButtonReleasedEvent);
		REGISTER_EVENT(MouseMovedEvent);
		REGISTER_EVENT(MouseScrolledEvent);
		REGISTER_EVENT(MouseRawInputEvent);
		REGISTER_EVENT(MouseInitializedEvent);

		// ----- WINDOW EVENTS
		REGISTER_EVENT(WindowResizedEvent);
		REGISTER_EVENT(WindowMovedEvent);
		REGISTER_EVENT(WindowClosedEvent);
		REGISTER_EVENT(WindowEnterSizeMoveEvent);
		REGISTER_EVENT(WindowExitSizeMoveEvent);
		REGISTER_EVENT(ActivateAppEvent);
		REGISTER_EVENT(ToggleFullscreenEvent);
	};
}
