#pragma once

#include "Event.h"

#include "../Core/KeyboardKeys.h"

namespace BuD
{
	class KeyDownEvent : public Event
	{
	public:
		KeyDownEvent(KeyboardKeys key) : m_key(key) {}

		virtual void Visit(IEventDispatchable&) override;

		const KeyboardKeys m_key;
	};

	class KeyReleaseEvent : public Event
	{
	public:
		KeyReleaseEvent(KeyboardKeys key) : m_key(key) {}

		virtual void Visit(IEventDispatchable&) override;

		const KeyboardKeys m_key;
	};

	class KeyRepeatEvent : public Event
	{
	public:
		KeyRepeatEvent(KeyboardKeys key) : m_key(key) {}

		virtual void Visit(IEventDispatchable&) override;

		const KeyboardKeys m_key;
	};
}
