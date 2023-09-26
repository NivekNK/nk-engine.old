#pragma once

/** @file */

#if !defined(NK_RELEASE)
    #define NK_ENABLE_ASSERT 1
#endif

#if NK_ENABLE_ASSERT

namespace nk {
    class Assertion {
    public:
        template <typename... Args>
        static inline void report_assert_failure(
            const cstr expression, bool is_empty_message, const cstr message, Args&&... args) {
            if (!is_empty_message) {
                str final_message;
                std::vformat_to(
                    std::back_inserter(final_message),
                    message,
                    std::make_format_args(std::forward<Args>(args)...));

                FatalLog("Assertion Failure: {}, message: '{}'",
                         expression, final_message);
            } else {
                FatalLog("Assertion Failure: {}",
                         expression);
            }
        }

        static inline void debug_break() {
    #if defined(NK_PLATFORM_LINUX)
            __builtin_trap();
    #elif defined(NK_PLATFORM_WINDOWS)
            __debugbreak();
    #else
            FatalLog("nk-engine only supports Windows and Linux!");
    #endif
        }
    };
}

    #define AssertMsg(expr, message, ...)                                                               \
        {                                                                                               \
            if (expr) {                                                                                 \
            } else {                                                                                    \
                nk::Assertion::report_assert_failure(#expr, false, message __VA_OPT__(, ) __VA_ARGS__); \
                nk::Assertion::debug_break();                                                           \
            }                                                                                           \
        }

    #define Assert(expr)                                               \
        {                                                              \
            if (expr) {                                                \
            } else {                                                   \
                nk::Assertion::report_assert_failure(#expr, true, ""); \
                nk::Assertion::debug_break();                          \
            }                                                          \
        }

    #define AssertKeep(expr)                  Assert(expr)

    #define AssertKeepMsg(expr, message, ...) AssertMsg(expr, message __VA_OPT__(, ) __VA_ARGS__)

#else

    #define Assert(expr)
    #define AssertMsg(expr, message)
    #define AssertKeep(expr)             expr
    #define AssertKeepMsg(expr, message) expr

#endif
