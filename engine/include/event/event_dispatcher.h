#pragma once

#include <eventpp/eventdispatcher.h>

#include "event/event.h"

#define BindFunc(Func) \
    [this](auto&& arg) { return Func(std::forward<decltype(arg)>(arg)); }

namespace nk {
    class EventDispatcher {
    private:
        struct EventPolicies {
            static EventType getEvent(const Event& event) {
                if (event.is_handled())
                    return EventType::None;

                return event.get_event_type();
            }
        };

        using DispatcherImpl = eventpp::EventDispatcher<EventType, void(Event&), EventPolicies>;

    public:
        using Handle = DispatcherImpl::Handle;

        template <std::derived_from<Event> T>
        using Callback = std::function<void(T&)>;

        EventDispatcher() = default;

        template <std::derived_from<Event> T>
        Handle add_listener(const Callback<T>& callback) {
            return m_dispatcher.appendListener(
                T::get_static_type(),
                [callback](Event& event) {
                    auto& e = dynamic_cast<T&>(event);
                    callback(e);
                }
            );
        }

        bool remove_listener(const EventType type, const Handle& handle) {
            return m_dispatcher.removeListener(type, handle);
        }

        template <std::derived_from<Event> T>
        bool remove_listener(const Handle& handle) {
            return m_dispatcher.removeListener(T::get_static_type(), handle);
        }

        void dispatch(Event& event) {
            m_dispatcher.dispatch(event);
        }

        void dispatch(Event& event, const EventType type) {
            if (event.get_event_type() == type)
                m_dispatcher.dispatch(event);
        }

        template <std::derived_from<Event> T>
        void dispatch(Event& event) {
            if (event.get_event_type() == T::get_static_type())
                m_dispatcher.dispatch(event);
        }

        template <std::derived_from<Event> T>
        void dispatch(T& event) {
            m_dispatcher.dispatch(event);
        }

    private:
        DispatcherImpl m_dispatcher;
    };
}
