#pragma once

#if defined(NK_RELEASE)
    #define NK_LOG_TRACE_ENABLED 0
    #define NK_LOG_DEBUG_ENABLED 0
#else
    #define NK_LOG_TRACE_ENABLED 1
    #define NK_LOG_DEBUG_ENABLED 1
#endif

#define NK_LOG_INFO_ENABLED 1
#define NK_LOG_WARN_ENABLED 1

#define NK_CREATE_LOG_FUNCTION(Type)                                       \
    template <typename... Args>                                            \
    void Type(const cstr file, const u32 line, cstr fmt, Args&&... args) { \
        if (m_priority > Priority::Priority##Type)                         \
            return;                                                        \
        str message;                                                       \
        std::vformat_to(                                                   \
            std::back_inserter(message),                                   \
            fmt,                                                           \
            std::make_format_args(std::forward<Args>(args)...));           \
        log(message.c_str(), #Type, Priority::Priority##Type, file, line); \
    }

namespace nk {
    class LoggingSystem {
    public:
        static void init() {
            LoggingSystem& system = get();
        }

        static LoggingSystem& get() {
            static LoggingSystem instance;
            return instance;
        }

        ~LoggingSystem() = default;

        enum Priority {
            PriorityTrace,
            PriorityDebug,
            PriorityInfo,
            PriorityWarn,
            PriorityError,
            PriorityFatal,
        };

        inline LoggingSystem& set_priority(Priority priority) {
            m_priority = priority;
            return *this;
        }
        inline LoggingSystem& set_file_output(bool enabled) {
            m_file_output = enabled;
            return *this;
        }

        void styled_log(const cstr message, i8 font, i8 background, i8 style);

        NK_CREATE_LOG_FUNCTION(Trace)
        NK_CREATE_LOG_FUNCTION(Debug)
        NK_CREATE_LOG_FUNCTION(Info)
        NK_CREATE_LOG_FUNCTION(Warn)
        NK_CREATE_LOG_FUNCTION(Error)
        NK_CREATE_LOG_FUNCTION(Fatal)

    private:
        LoggingSystem()
            : m_file_output{true},
#if defined(NK_RELEASE)
              m_priority{Priority::PriorityInfo},
#else
              m_priority{Priority::PriorityTrace}
#endif
        {
        }

#if defined(NK_PLATFORM_WINDOWS)
        void activate_virtual_terminal();
#endif
        cstr colorize(i8 font, i8 background = -1, i8 style = -1);
        void log(const cstr message, const cstr log_name, const Priority priority, const cstr file, const u32 line);

        Priority m_priority = Priority::PriorityInfo;
        bool m_file_output = true;
        std::mutex m_log_mutex;

        static constexpr const i8 m_style[Priority::PriorityFatal + 1][3] = {
            {-1, -1, -1},
            {2, -1, -1},
            {6, -1, -1},
            {3, -1, 1},
            {1, -1, 1},
            {-1, 1, 1},
        };
    };
}

#if NK_LOG_TRACE_ENABLED == 1
    #define TraceLog(...) ::nk::LoggingSystem::get().Trace(__FILE__, __LINE__, __VA_ARGS__)
    #define TraceIfLog(value, ...) \
        if (value)                 \
        ::nk::LoggingSystem::get().Trace(__FILE__, __LINE__, __VA_ARGS__)
#else
    #define TraceLog(...)
    #define TraceIfLog(value, ...)
#endif

#if NK_LOG_DEBUG_ENABLED == 1
    #define DebugLog(...) ::nk::LoggingSystem::get().Debug(__FILE__, __LINE__, __VA_ARGS__)
    #define DebugIfLog(value, ...) \
        if (value)                 \
        ::nk::LoggingSystem::get().Debug(__FILE__, __LINE__, __VA_ARGS__)
#else
    #define DebugLog(...)
    #define DebugIfLog(value, ...)
#endif

#if NK_LOG_INFO_ENABLED == 1
    #define InfoLog(...) ::nk::LoggingSystem::get().Info(__FILE__, __LINE__, __VA_ARGS__)
    #define InfoIfLog(value, ...) \
        if (value)                \
        ::nk::LoggingSystem::get().Info(__FILE__, __LINE__, __VA_ARGS__)
#else
    #define InfoLog(...)
    #define InfoIfLog(value, ...)
#endif

#if NK_LOG_WARN_ENABLED == 1
    #define WarnLog(...) ::nk::LoggingSystem::get().Warn(__FILE__, __LINE__, __VA_ARGS__)
    #define WarnIfLog(value, ...) \
        if (value)                \
        ::nk::LoggingSystem::get().Warn(__FILE__, __LINE__, __VA_ARGS__)
#else
    #define WarnLog(...)
    #define WarnIfLog(value, ...)
#endif

#define ErrorLog(...) ::nk::LoggingSystem::get().Error(__FILE__, __LINE__, __VA_ARGS__)
#define ErrorIfLog(value, ...) \
    if (value)                 \
    ::nk::LoggingSystem::get().Error(__FILE__, __LINE__, __VA_ARGS__)

#define FatalLog(...) ::nk::LoggingSystem::get().Fatal(__FILE__, __LINE__, __VA_ARGS__)
#define FatalIfLog(value, ...) \
    if (value)                 \
    ::nk::LoggingSystem::get().Fatal(__FILE__, __LINE__, __VA_ARGS__)

#define StyledLog(message, font, background, style) \
    ::nk::LoggingSystem::get().styled_log(message, font, background, style)
