#pragma once

namespace nk {
    enum class EventType {
        None = 0,
        WindowClose,
        WindowResize,
        WindowGainedFocus,
        WindowLostFocus,
        WindowMoved,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,

        MaxTypes = 255
    };

    class Event {
    public:
        virtual EventType get_event_type() const = 0;
        virtual const str get_name() const = 0;
        virtual str to_string() const { return get_name(); }

        bool is_handled() const { return m_handled; }
        void handle() { m_handled = true; }
        void reset() { m_handled = false; }

    private:
        bool m_handled = false;
    };
}

#define DefineEventClassType(Type)                                                \
    static inline nk::EventType get_static_type() { return nk::EventType::Type; } \
    virtual nk::EventType get_event_type() const override {                       \
        return get_static_type();                                                 \
    }                                                                             \
    virtual const str get_name() const override { return #Type; }

template <std::derived_from<nk::Event> T, typename CharT>
struct std::formatter<T, CharT> : std::formatter<std::string> {
    template <typename FormatContext>
    auto format(T& e, FormatContext& ctx) const {
        return std::formatter<std::string>::format(e.to_string(), ctx);
    }
};
