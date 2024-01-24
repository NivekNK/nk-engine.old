#pragma once

#include "memory/allocator.h"

namespace nk {
    template <typename D, typename T>
    concept IDyarr = requires(D& dyarr) {
        { dyarr.data() } -> std::same_as<T*>;
        { dyarr.length() } -> std::same_as<u64>;
        { dyarr.push_use() } -> std::same_as<T&>;
    };

    template <typename T>
    class Arr {
    public:
        Arr() : m_data{nullptr}, m_length{0}, m_allocator{nullptr} {}

        Arr(const Arr&) = delete;
        Arr& operator=(const Arr&) = delete;

        void init(const u64 length, Allocator* allocator, bool own_allocator = true) {
            m_length = length;
            m_allocator = allocator;
            m_data = m_allocator->allocate<T>(m_length);
            m_own_allocator = own_allocator;
        }

        void init(T* data, const u64 length) {
            m_data = data;
            m_length = length;
            m_allocator = nullptr;
            m_own_allocator = false;
        }

        template <IDyarr<T> D>
        void init(D& dyarr) {
            m_data = dyarr.data();
            m_length = dyarr.length();
            m_allocator = nullptr;
            m_own_allocator = false;
        }

        void init(std::initializer_list<T> data, Allocator* allocator, bool own_allocator = true) {
            m_length = data.size();
            m_allocator = allocator;
            m_own_allocator = own_allocator;
            m_data = m_allocator->allocate<T>(m_length);
            std::uninitialized_copy(data.begin(), data.end(), m_data);
        }

        Arr(const u16 length, Allocator* allocator, bool own_allocator = true)
            : m_length{length}, m_allocator{allocator}, m_own_allocator{own_allocator} {
            m_data = m_allocator->allocate<T>(m_length);
        }

        Arr(T* data, const u64 length)
            : m_data{data}, m_length{length}, m_allocator{nullptr}, m_own_allocator{false} {
        }

        template <IDyarr<T> D>
        Arr(D& dyarr)
            : m_data{dyarr.data()}, m_length{dyarr.length()}, m_allocator{nullptr}, m_own_allocator{false} {
        }

        Arr(std::initializer_list<T> data, Allocator* allocator, bool own_allocator = true)
            : m_data{nullptr},
              m_length{data.size()},
              m_allocator{allocator},
              m_own_allocator{own_allocator} {
            m_data = m_allocator->allocate<T>(m_length);
            std::uninitialized_copy(data.begin(), data.end(), m_data);
        }

        Arr(Arr<T>&& other) noexcept {
            m_data = other.m_data;
            m_length = other.m_length;
            m_allocator = other.m_allocator;
            m_own_allocator = other.m_own_allocator;

            other.m_data = nullptr;
            other.m_length = 0;
            other.m_allocator = nullptr;
            other.m_own_allocator = false;
        }

        Arr& operator=(Arr&& right) noexcept {
            m_data = right.m_data;
            m_length = right.m_length;
            m_allocator = right.m_allocator;
            m_own_allocator = right.m_own_allocator;

            right.m_data = nullptr;
            right.m_length = 0;
            right.m_allocator = nullptr;
            right.m_own_allocator = false;

            return *this;
        };

        inline ~Arr() { free(); }

        T& operator[](const u64 index) {
            ErrorIfLog(index >= m_length, "At index: {} with length: {}!", index, m_length);
            return m_data[index];
        }

        const T& operator[](const u64 index) const {
            ErrorIfLog(index >= m_length, "At index: {} with length: {}!", index, m_length);
            return m_data[index];
        }

        u64 length() const { return m_length; }
        bool empty() const { return m_length == 0; }
        T* data() { return m_data; }
        void free() {
            if (m_allocator != nullptr) {
                m_allocator->free(m_data, m_length * sizeof(T));
                if (m_own_allocator)
                    delete m_allocator;
                m_allocator = nullptr;
            }
        }
        void free(Allocator* allocator) { allocator->free(m_data, m_length * sizeof(T)); }

        struct Iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;

            Iterator(pointer ptr) : m_ptr{ptr} {}

            reference operator*() const { return *m_ptr; }
            pointer operator->() { return m_ptr; }

            Iterator& operator++() {
                m_ptr++;
                return *this;
            }

            Iterator operator++(int) {
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const Iterator& a, const Iterator& b) {
                return a.m_ptr == b.m_ptr;
            }

            friend bool operator!=(const Iterator& a, const Iterator& b) {
                return a.m_ptr != b.m_ptr;
            }

        private:
            pointer m_ptr;
        };

        Iterator begin() { return Iterator{&m_data[0]}; }
        Iterator end() { return Iterator{&m_data[m_length]}; }
        Iterator begin() const { return Iterator{&m_data[0]}; }
        Iterator end() const { return Iterator{&m_data[m_length]}; }

    private:
        T* m_data = nullptr;
        u64 m_length = 0;
        Allocator* m_allocator = nullptr;
        bool m_own_allocator = false;
    };
}
