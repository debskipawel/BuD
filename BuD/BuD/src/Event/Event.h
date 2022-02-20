#pragma once

namespace BuD
{
#define OVERRIDE_EVENT(EventName); void EventName::Visit(IEventDispatchable& e) { e.OnConcreteEvent(*this); }
#define REGISTER_EVENT(EventName); inline virtual void OnConcreteEvent(EventName&) { }

	class IEventDispatchable;

	class Event
	{
	public:

		virtual void Visit(IEventDispatchable&) = 0;
		virtual ~Event() = default;

	protected:
		Event();
	};
}
