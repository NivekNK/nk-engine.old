#pragma once

#include "core/map_macro.h"

namespace nk {
    template <typename T>
    struct EnableEnumClassBitMask {
        static constexpr bool value = false;
    };

#define NK_BIT_MASK_MAP_LIST0(f, x, peek, ...) f(NK_COUNT_ARGUMENTS(__VA_ARGS__), x) MAP_LIST_NEXT(peek, NK_BIT_MASK_MAP_LIST1)(f, peek, __VA_ARGS__)
#define NK_BIT_MASK_MAP_LIST1(f, x, peek, ...) f(NK_COUNT_ARGUMENTS(__VA_ARGS__), x) MAP_LIST_NEXT(peek, NK_BIT_MASK_MAP_LIST0)(f, peek, __VA_ARGS__)
#define NK_BIT_MASK_MAP_LIST(f, ...)           EVAL(NK_BIT_MASK_MAP_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#define NK_BIT_MASK_CREATE(count, name)        name = 0x1 << count

#define DefineEnumBitMask(Name, Values)                                                     \
    enum class Name {                                                                       \
        None = 0,                                                                           \
        NK_BIT_MASK_MAP_LIST(NK_BIT_MASK_CREATE, NK_EXPAND_ARGUMENTS Values)                \
    };                                                                                      \
    template <>                                                                             \
    struct EnableEnumClassBitMask<Name> {                                                   \
        static constexpr bool value = true;                                                 \
    };                                                                                      \
    namespace Name##Impl {                                                                  \
        static inline constexpr u32 count = NK_COUNT_ARGUMENTS(NK_EXPAND_ARGUMENTS Values); \
    }

    template <typename T>
    struct EnumWrapper {
        constexpr EnumWrapper(const T& value)
            : value{value} {}

        constexpr explicit operator bool() const {
            using UT = typename std::underlying_type<T>::type;
            return static_cast<UT>(value) != 0;
        }

        constexpr operator T() const {
            return value;
        }

        T value;
    };

    template <typename T>
    struct BitMask {
        using underlying_type = typename std::underlying_type<T>::type;

        constexpr BitMask(const T& value)
            : value{static_cast<underlying_type>(value)} {}

        constexpr BitMask(const EnumWrapper<T>& enumerator)
            : value{static_cast<underlying_type>(enumerator.value)} {}

        constexpr explicit operator bool() const {
            return value != 0;
        }

        underlying_type value;
    };

#define TypenameEnableBitMask \
    constexpr typename std::enable_if<std::is_enum<T>::value && EnableEnumClassBitMask<T>::value, BitMask<T>>::type

#define TypenameEnableBitMaskRef \
    constexpr typename std::enable_if<std::is_enum<T>::value && EnableEnumClassBitMask<T>::value, BitMask<T>&>::type

#define TypenameEnableEnumWrapper \
    constexpr typename std::enable_if<std::is_enum<T>::value && EnableEnumClassBitMask<T>::value, EnumWrapper<T>>::type

#define TypenameEnableBool \
    constexpr typename std::enable_if<std::is_enum<T>::value && EnableEnumClassBitMask<T>::value, bool>::type

    template <typename T>
    TypenameEnableBitMask make_bit_mask(const T& t) {
        return BitMask<T>{t};
    }

    /**
     * operator&(T, T)
     */
    template <typename T>
    TypenameEnableEnumWrapper operator&(const T& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        Assert((static_cast<UT>(lhs) & (static_cast<UT>(lhs) - 1)) == 0);
        return EnumWrapper<T>{static_cast<T>(static_cast<UT>(lhs) & static_cast<UT>(rhs))};
    }

    /**
     * operator&(EnumWrapper<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableEnumWrapper operator&(const EnumWrapper<T>& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return EnumWrapper<T>{static_cast<T>(static_cast<UT>(lhs.value) & static_cast<UT>(rhs.value))};
    }

    /**
     * operator&(BitMask<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator&(const BitMask<T>& lhs, const BitMask<T>& rhs) {
        return BitMask<T>{static_cast<T>(lhs.value & rhs.value)};
    }

    /**
     * operator&(BitMask<T>, T)
     */
    template <typename T>
    TypenameEnableEnumWrapper operator&(const BitMask<T>& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return EnumWrapper<T>{static_cast<T>(lhs.value & static_cast<UT>(rhs))};
    }

    /**
     * operator&(T, BitMask<T>)
     */
    template <typename T>
    TypenameEnableEnumWrapper operator&(const T& lhs, const BitMask<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return EnumWrapper<T>{static_cast<T>(static_cast<UT>(lhs) & rhs.value)};
    }

    /**
     * operator&(BitMask<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableEnumWrapper operator&(const BitMask<T>& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return EnumWrapper<T>{static_cast<T>(lhs.value & static_cast<UT>(rhs.value))};
    }

    /**
     * operator&(EnumWrapper<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableEnumWrapper operator&(const EnumWrapper<T>& lhs, const BitMask<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return EnumWrapper<T>{static_cast<T>(static_cast<UT>(lhs.value) & rhs.value)};
    }

    /**
     * operator&(T, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableEnumWrapper operator&(const T& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return EnumWrapper<T>{static_cast<T>(static_cast<UT>(lhs) & static_cast<UT>(rhs.value))};
    }

    /**
     * operator&(EnumWrapper<T>, T)
     */
    template <typename T>
    TypenameEnableEnumWrapper operator&(const EnumWrapper<T>& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return EnumWrapper<T>{static_cast<T>(static_cast<UT>(lhs.value) & static_cast<UT>(rhs))};
    }

    /**
     * operator|(T, T)
     */
    template <typename T>
    TypenameEnableBitMask operator|(const T& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs) | static_cast<UT>(rhs))};
    }

    /**
     * operator|(EnumWrapper<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator|(const EnumWrapper<T>& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs.value) | static_cast<UT>(rhs.value))};
    }

    /**
     * operator|(BitMask<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator|(const BitMask<T>& lhs, const BitMask<T>& rhs) {
        return BitMask<T>{static_cast<T>(lhs.value | rhs.value)};
    }

    /**
     * operator|(BitMask<T>, T)
     */
    template <typename T>
    TypenameEnableBitMask operator|(const BitMask<T>& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(lhs.value | static_cast<UT>(rhs))};
    }

    /**
     * operator|(T, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator|(const T& lhs, const BitMask<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs) | rhs.value)};
    }

    /**
     * operator|(BitMask<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator|(const BitMask<T>& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(lhs.value | static_cast<UT>(rhs.value))};
    }

    /**
     * operator|(EnumWrapper<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator|(const EnumWrapper<T>& lhs, const BitMask<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs.value) | rhs.value)};
    }

    /**
     * operator|(EnumWrapper<T>, T)
     */
    template <typename T>
    TypenameEnableBitMask operator|(const EnumWrapper<T>& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs.value) | static_cast<UT>(rhs))};
    }

    /**
     * operator|(T, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator|(const T& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs) | static_cast<UT>(rhs.value))};
    }

    /**
     * operator^(T, T)
     */
    template <typename T>
    TypenameEnableBitMask operator^(const T& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs) ^ static_cast<UT>(rhs))};
    }

    /**
     * operator^(EnumWrapper<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator^(const EnumWrapper<T>& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs.value) ^ static_cast<UT>(rhs.value))};
    }

    /**
     * operator^(BitMask<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator^(const BitMask<T>& lhs, const BitMask<T>& rhs) {
        return BitMask<T>{static_cast<T>(lhs.value ^ rhs.value)};
    }

    /**
     * operator^(BitMask<T>, T)
     */
    template <typename T>
    TypenameEnableBitMask operator^(const BitMask<T>& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(lhs.value ^ static_cast<UT>(rhs))};
    }

    /**
     * operator^(T, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator^(const T& lhs, const BitMask<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs) ^ rhs.value)};
    }

    /**
     * operator^(BitMask<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator^(const BitMask<T>& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(lhs.value ^ static_cast<UT>(rhs.value))};
    }

    /**
     * operator^(EnumWrapper<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator^(const EnumWrapper<T>& lhs, const BitMask<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs.value) ^ rhs.value)};
    }

    /**
     * operator^(EnumWrapper<T>, T)
     */
    template <typename T>
    TypenameEnableBitMask operator^(const EnumWrapper<T>& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs.value) ^ static_cast<UT>(rhs))};
    }

    /**
     * operator^(T, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator^(const T& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(static_cast<UT>(lhs) ^ static_cast<UT>(rhs.value))};
    }

    /**
     * operator~(T)
     */
    template <typename T>
    TypenameEnableBitMask operator~(const T& value) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(~static_cast<UT>(value))};
    }

    /**
     * operator~(EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator~(const EnumWrapper<T>& lhs) {
        using UT = typename std::underlying_type<T>::type;
        return BitMask<T>{static_cast<T>(~static_cast<UT>(lhs.value))};
    }

    /**
     * operator~(BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMask operator~(const BitMask<T>& lhs) {
        return BitMask<T>{static_cast<T>(~lhs.value)};
    }

    /**
     * operator&=(BitMask<T>, T)
     */
    template <typename T>
    TypenameEnableBitMaskRef operator&=(BitMask<T>& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        lhs.value &= static_cast<UT>(rhs);
        return lhs;
    }

    /**
     * operator&=(BitMask<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMaskRef operator&=(BitMask<T>& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        lhs.value &= static_cast<UT>(rhs.value);
        return lhs;
    }

    /**
     * operator&=(BitMask<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMaskRef operator&=(BitMask<T>& lhs, const BitMask<T>& rhs) {
        lhs.value &= rhs.value;
        return lhs;
    }

    /**
     * operator|=(BitMask<T>, T)
     */
    template <typename T>
    TypenameEnableBitMaskRef operator|=(BitMask<T>& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        lhs.value |= static_cast<UT>(rhs);
        return lhs;
    }

    /**
     * operator|=(BitMask<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMaskRef operator|=(BitMask<T>& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        lhs.value |= static_cast<UT>(rhs.value);
        return lhs;
    }

    /**
     * operator|=(BitMask<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMaskRef operator|=(BitMask<T>& lhs, const BitMask<T>& rhs) {
        lhs.value |= rhs.value;
        return lhs;
    }

    /**
     * operator^=(BitMask<T>, T)
     */
    template <typename T>
    TypenameEnableBitMaskRef operator^=(BitMask<T>& lhs, const T& rhs) {
        using UT = typename std::underlying_type<T>::type;
        lhs.value ^= static_cast<UT>(rhs);
        return lhs;
    }

    /**
     * operator^=(BitMask<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBitMaskRef operator^=(BitMask<T>& lhs, const EnumWrapper<T>& rhs) {
        using UT = typename std::underlying_type<T>::type;
        lhs.value ^= static_cast<UT>(rhs.value);
        return lhs;
    }

    /**
     * operator^=(BitMask<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBitMaskRef operator^=(BitMask<T>& lhs, const BitMask<T>& rhs) {
        lhs.value ^= rhs.value;
        return lhs;
    }

    /**
     * operator==(T, T)
     */
    template <typename T>
    TypenameEnableBool operator==(const T& lhs, const T& rhs) {
        return lhs == rhs;
    }

    /**
     * operator==(EnumWrapper<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBool operator==(const EnumWrapper<T>& lhs, const EnumWrapper<T>& rhs) {
        return lhs.value == rhs.value;
    }

    /**
     * operator==(BitMask<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBool operator==(const BitMask<T>& lhs, const BitMask<T>& rhs) {
        return lhs.value == rhs.value;
    }

    /**
     * operator==(EnumWrapper<T>, T)
     */
    template <typename T>
    TypenameEnableBool operator==(const EnumWrapper<T>& lhs, const T& rhs) {
        return lhs.value == rhs;
    }

    /**
     * operator==(T, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBool operator==(const T& lhs, const EnumWrapper<T>& rhs) {
        return lhs == rhs.value;
    }

    /**
     * operator==(BitMask<T>, T)
     */
    template <typename T>
    TypenameEnableBool operator==([[maybe_unused]] const BitMask<T>& lhs, [[maybe_unused]] const T& rhs) {
        static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value,
                      "A BitMask can't be compared to an Enum. Use & first.");
        return false;
    }

    /**
     * operator==(T, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBool operator==([[maybe_unused]] const T& lhs, [[maybe_unused]] const BitMask<T>& rhs) {
        static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value,
                      "A BitMask can't be compared to an Enum. Use & first.");
        return false;
    }

    /**
     * operator==(BitMask<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBool operator==([[maybe_unused]] const BitMask<T>& lhs, [[maybe_unused]] const EnumWrapper<T>& rhs) {
        static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value,
                      "A BitMask can't be compared to an Enum. Use & first.");
        return false;
    }

    /**
     * operator==(EnumWrapper<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBool operator==([[maybe_unused]] const EnumWrapper<T>& lhs, [[maybe_unused]] const BitMask<T>& rhs) {
        static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value,
                      "A BitMask can't be compared to an Enum. Use & first.");
        return false;
    }

    /**
     * operator!=(T, T)
     */
    template <typename T>
    TypenameEnableBool operator!=(const T& lhs, const T& rhs) {
        return lhs != rhs;
    }

    /**
     * operator!=(EnumWrapper<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBool operator!=(const EnumWrapper<T>& lhs, const EnumWrapper<T>& rhs) {
        return lhs.value != rhs.value;
    }

    /**
     * operator!=(BitMask<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBool operator!=(const BitMask<T>& lhs, const BitMask<T>& rhs) {
        return lhs.value != rhs.value;
    }

    /**
     * operator!=(EnumWrapper<T>, T)
     */
    template <typename T>
    TypenameEnableBool operator!=(const EnumWrapper<T>& lhs, const T& rhs) {
        return lhs.value != rhs;
    }

    /**
     * operator!=(T, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBool operator!=(const T& lhs, const EnumWrapper<T>& rhs) {
        return lhs != rhs.value;
    }

    /**
     * operator!=(BitMask<T>, T)
     */
    template <typename T>
    TypenameEnableBool operator!=([[maybe_unused]] const BitMask<T>& lhs, [[maybe_unused]] const T& rhs) {
        static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value,
                      "A BitMask can't be compared to an Enum. Use & first.");
        return false;
    }

    /**
     * operator!=(T, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBool operator!=([[maybe_unused]] const T& lhs, [[maybe_unused]] const BitMask<T>& rhs) {
        static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value,
                      "A BitMask can't be compared to an Enum. Use & first.");
        return false;
    }

    /**
     * operator!=(BitMask<T>, EnumWrapper<T>)
     */
    template <typename T>
    TypenameEnableBool operator!=([[maybe_unused]] const BitMask<T>& lhs, [[maybe_unused]] const EnumWrapper<T>& rhs) {
        static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value,
                      "A BitMask can't be compared to an Enum. Use & first.");
        return false;
    }

    /**
     * operator!=(EnumWrapper<T>, BitMask<T>)
     */
    template <typename T>
    TypenameEnableBool operator!=([[maybe_unused]] const EnumWrapper<T>& lhs, [[maybe_unused]] const BitMask<T>& rhs) {
        static_assert(!std::is_same<typename BitMask<T>::underlying_type, typename std::underlying_type<T>::type>::value,
                      "A BitMask can't be compared to an Enum. Use & first.");
        return false;
    }
}
