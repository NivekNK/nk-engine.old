#pragma once

namespace nk {
    template <typename D, typename T>
    concept IDyarr = requires(D& dyarr) {
        { dyarr.data() } -> std::same_as<T*>;
        { dyarr.length() } -> std::same_as<u64>;
    };

    template <typename T>
    class Arr {
    public:
        Arr(T* data, const u64 length)
            : m_data{data}, m_length{length} {
        }

        template <IDyarr<T> D>
        Arr(D& dyarr)
            : m_data{dyarr.data()}, m_length{dyarr.length()} {
        }

        T& operator[](const u64 index) {
            ErrorIfLog(index >= m_length, "At index: {} with length: {}!", index, m_length);
            return m_data[index];
        }

        const T& operator[](const u64 index) const {
            ErrorIfLog(index >= m_length, "At index: {} with length: {}!", index, m_length);
            return m_data[index];
        }

        u64 length() const { return m_length; }

    private:
        T* m_data = nullptr;
        u64 m_length = 0;
    };
}
