#pragma once

#include "Event.h"
#include "../Core/MouseCode.h"

namespace BuD
{
	class MouseButtonDownEvent : public Event
	{
	public:
		MouseButtonDownEvent(MouseCode button) : m_button(button) {}

		virtual void Visit(IEventDispatchable&) override;

		const MouseCode m_button;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(MouseCode button) : m_button(button) {}

		virtual void Visit(IEventDispatchable&) override;

		const MouseCode m_button;
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int xPos, int yPos) : m_xPos(xPos), m_yPos(yPos) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_xPos, m_yPos;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent() {}

		virtual void Visit(IEventDispatchable&) override;
	};

	class MouseRawInputEvent : public Event
	{
	public:
		MouseRawInputEvent() {}

		virtual void Visit(IEventDispatchable&) override;
	};

	class MouseInitializedEvent : public Event
	{
	public:
		MouseInitializedEvent() {}

		virtual void Visit(IEventDispatchable&) override;
	};
}
	