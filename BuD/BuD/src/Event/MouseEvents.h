#pragma once

#include "Event.h"


namespace BuD
{
	class MouseButtonDownEvent : public Event
	{
	public:
		MouseButtonDownEvent(int button): m_button(button)
		{
		}

		virtual void Visit(IEventDispatchable&) override;

		const int m_button;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(int button) : Event(), m_button(button) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_button;
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int xPos, int yPos) : Event(), m_xPos(xPos), m_yPos(yPos) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_xPos, m_yPos;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent() : Event() {}

		virtual void Visit(IEventDispatchable&) override;
	};

	class MouseRawInputEvent : public Event
	{
	public:
		MouseRawInputEvent() : Event() {}

		virtual void Visit(IEventDispatchable&) override;
	};

	class MouseInitializedEvent : public Event
	{
	public:
		MouseInitializedEvent() : Event() {}

		virtual void Visit(IEventDispatchable&) override;
	};
}
	