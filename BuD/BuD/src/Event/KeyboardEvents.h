#pragma once

#include "Event.h"

namespace BuD
{
	class KeyboardEvent : public Event
	{
	protected:
		KeyboardEvent(int key) : m_key(key) {}

	public:
		const int m_key;
	};

	class KeyDownEvent : public KeyboardEvent
	{
	public:
		KeyDownEvent(int key) : KeyboardEvent(key) {}

		virtual void Visit(IEventDispatchable&) override;
	};

	class KeyReleaseEvent : public KeyboardEvent
	{
	public:
		KeyReleaseEvent(int key) : KeyboardEvent(key) {}

		virtual void Visit(IEventDispatchable&) override;
	};

	class KeyRepeatEvent : public KeyboardEvent
	{
	public:
		KeyRepeatEvent(int key) : KeyboardEvent(key) {}

		virtual void Visit(IEventDispatchable&) override;
	};
}
