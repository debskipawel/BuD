#pragma once

#include "Event.h"


namespace BuD
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(int width, int height) : Event(), m_width(width), m_height(height) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_width, m_height;
	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int xPos, int yPos) : Event(), m_xPos(xPos), m_yPos(yPos) {}

		virtual void Visit(IEventDispatchable&) override;

		const int m_xPos, m_yPos;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent()
			: Event()
		{
		}

		virtual void Visit(IEventDispatchable&) override;
	};

	class WindowEnterSizeMoveEvent : public Event
	{
	public:
		WindowEnterSizeMoveEvent() : Event() {}

		virtual void Visit(IEventDispatchable&) override;
	};

	class WindowExitSizeMoveEvent : public Event
	{
	public:
		WindowExitSizeMoveEvent() : Event() {}

		virtual void Visit(IEventDispatchable&) override;
	};

	class ActivateAppEvent : public Event
	{
	public:
		ActivateAppEvent(bool isActive) : Event(), m_isActive(isActive) {}

		virtual void Visit(IEventDispatchable&) override;

		const bool m_isActive;
	};

	class ToggleFullscreenEvent : public Event
	{
	public:
		ToggleFullscreenEvent() : Event() {}

		virtual void Visit(IEventDispatchable&) override;
	};
}
