#pragma once

#include "Event.h"
#include "../Core/MouseCode.h"

namespace BuD
{
	class MouseButtonDownEvent : public Event
	{
	public:
		MouseButtonDownEvent(MouseCode button, int xPos, int yPos) : m_button(button), m_xPos(xPos), m_yPos(yPos) {}

		virtual void Visit(IEventDispatchable&) override;

		const MouseCode m_button;
		const int m_xPos, m_yPos;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(MouseCode button, int xPos, int yPos) : m_button(button), m_xPos(xPos), m_yPos(yPos) {}

		virtual void Visit(IEventDispatchable&) override;

		const MouseCode m_button;
		const int m_xPos, m_yPos;
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int xOffset, int yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_xOffset, m_yOffset;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(int xPos, int yPos, int wheelDelta) : m_xPos(xPos), m_yPos(yPos), m_wheelDelta(wheelDelta) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_xPos, m_yPos, m_wheelDelta;
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
	