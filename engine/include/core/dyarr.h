#pragma once

#include "memory/allocator.h"

namespace nk {
    template <typename T>
    class Dyarr {
    public:
        Dyarr(Allocator* allocator, const u64 initial_capacity, const u64 initial_length = 0)
            : m_data{nullptr},
              m_length{initial_length},
              m_capacity{0},
              m_allocator{allocator} {
            if (initial_capacity > 0)
                grow(initial_capacity);
        }

        Dyarr(Allocator* allocator, std::initializer_list<T> data)
            : m_data{nullptr},
              m_length{data.size()},
              m_capacity{0},
              m_allocator{allocator} {
            if (data.size() > 0)
                grow(data.size());
            std::copy(data.begin(), data.end(), m_data);
        }

        ~Dyarr() {
            if (m_capacity > 0) {
                m_allocator->free(m_data);
            }
        }

        void push(const T& element);
        // Return empty T to be filled
        T& push_use();

        void insert(const T& element, const u64 index);
        T& insert_use(const u64 index);

        void pop();
        // Delete element at index and swap it with the last element on the array
        void remove_swap_tail(const u64 index);
        void remove(const u64 index);

        using OptRefValue = std::optional<std::reference_wrapper<T>>;
        using ConstOptRefValue = std::optional<const std::reference_wrapper<T>>;

        OptRefValue operator[](const u64 index);
        ConstOptRefValue operator[](const u64 index) const;

        OptRefValue tail();
        ConstOptRefValue tail() const;

        OptRefValue head();
        ConstOptRefValue head() const;

        void clear();
        void set_length(const u64 length);
        void set_capacity(const u64 capacity);
        void grow(u64 capacity);

        u64 length_in_bytes() const;
        u64 capacity_in_bytes() const;

        u64 length() const;
        u64 capacity() const;

        T* data();

    private:
        T* m_data = nullptr;
        u64 m_length = 0;
        u64 m_capacity = 0;
        Allocator* m_allocator = nullptr;
    };

    template <typename T>
    inline void Dyarr<T>::push(const T& element) {
        if (m_length >= m_capacity)
            grow(m_capacity + 1);
        m_data[m_length++] = element;
    }

    template <typename T>
    inline T& Dyarr<T>::push_use() {
        if (m_length >= m_capacity)
            grow(m_capacity + 1);
        return m_data[m_length++];
    }

    template <typename T>
    inline void Dyarr<T>::insert(const T& element, const u64 index) {
        if (index >= m_capacity) {
            m_length = index + 1;
            grow(m_length);
            m_data[index] = element;
            return;
        }

        if (index >= m_length) {
            m_length = index + 1;
            m_data[index] = element;
            return;
        }

        m_length++;
        memmove(&m_data[index + 1], &m_data[index], sizeof *(m_data) * (m_length - 1 - index));
        m_data[index] = element;
    }

    template <typename T>
    inline T& Dyarr<T>::insert_use(const u64 index) {
        if (index >= m_capacity) {
            m_length = index + 1;
            grow(m_length);
            return m_data[index];
        }

        if (index >= m_length) {
            m_length = index + 1;
            return m_data[index];
        }

        m_length++;
        memmove(&m_data[index + 1], &m_data[index], sizeof *(m_data) * (m_length - 1 - index));
        return m_data[index];
    }

    template <typename T>
    inline void Dyarr<T>::pop() {
        WarnIfLog(m_length <= 0, "Trying to pop empty Dyarr!");
        if (m_length > 0)
            m_length--;
    }

    template <typename T>
    inline void Dyarr<T>::remove_swap_tail(const u64 index) {
        ErrorIfLog(index >= m_length, "Trying to remove_swap_tail at index: {} when there is only {} length.", index, m_length);
        if (index >= m_length)
            return;

        m_length--;
        m_data[index] = m_data[m_length];
    }

    template <typename T>
    inline void Dyarr<T>::remove(const u64 index) {
        WarnIfLog(index >= m_length, "Trying to remove at index: {} when there is only {} length.", index, m_length);
        if (index >= m_length)
            return;

        memmove(&m_data[index], &m_data[index + 1], sizeof *(m_data) * (m_length - 1 - index));
        m_length--;
    }

    template <typename T>
    inline Dyarr<T>::OptRefValue Dyarr<T>::operator[](const u64 index) {
        ErrorIfLog(index >= m_length, "At index: {} when there is only {} length.", index, m_length);
        if (index >= m_length)
            return std::nullopt;

        return m_data[index];
    }

    template <typename T>
    inline Dyarr<T>::ConstOptRefValue Dyarr<T>::operator[](const u64 index) const {
        ErrorIfLog(index >= m_length, "At index: {} when there is only {} length.", index, m_length);
        if (index >= m_length)
            return std::nullopt;

        return m_data[index];
    }

    template <typename T>
    inline Dyarr<T>::OptRefValue Dyarr<T>::tail() {
        ErrorIfLog(m_length <= 0, "Trying to get tail when Dyarr is empty.");
        if (m_length <= 0)
            return std::nullopt;

        return m_data[m_length - 1];
    }

    template <typename T>
    inline Dyarr<T>::ConstOptRefValue Dyarr<T>::tail() const {
        ErrorIfLog(m_length <= 0, "Trying to get tail when Dyarr is empty.");
        if (m_length <= 0)
            return std::nullopt;

        return m_data[m_length - 1];
    }

    template <typename T>
    inline Dyarr<T>::OptRefValue Dyarr<T>::head() {
        ErrorIfLog(m_length <= 0, "Trying to get head when Dyarr is empty.");
        if (m_length <= 0)
            return std::nullopt;

        return m_data[0];
    }

    template <typename T>
    inline Dyarr<T>::ConstOptRefValue Dyarr<T>::head() const {
        ErrorIfLog(m_length <= 0, "Trying to get head when Dyarr is empty.");
        if (m_length <= 0)
            return std::nullopt;

        return m_data[0];
    }

    template <typename T>
    inline void Dyarr<T>::clear() {
        m_length = 0;
    }

    template <typename T>
    inline void Dyarr<T>::set_length(const u64 length) {
        if (length > m_capacity)
            grow(length);
        m_length = length;
    }

    template <typename T>
    inline void Dyarr<T>::set_capacity(const u64 capacity) {
        if (capacity > m_capacity)
            grow(capacity);
    }

    template <typename T>
    inline void Dyarr<T>::grow(u64 capacity) {
        if (capacity < m_capacity * 2) {
            capacity = m_capacity * 2;
        } else if (capacity < 4) {
            capacity = 4;
        }

        T* data = static_cast<T*>(m_allocator->allocate(capacity * sizeof(T), alignof(T)));
        if (m_capacity > 0) {
            memcpy(data, m_data, m_capacity * sizeof(T));
            m_allocator->free(m_data);
        }

        m_data = data;
        m_capacity = capacity;
    }

    template <typename T>
    inline u64 Dyarr<T>::length_in_bytes() const {
        return m_length * sizeof(T);
    }

    template <typename T>
    inline u64 Dyarr<T>::capacity_in_bytes() const {
        return m_capacity * sizeof(T);
    }

    template <typename T>
    inline u64 Dyarr<T>::length() const {
        return m_length;
    }

    template <typename T>
    inline u64 Dyarr<T>::capacity() const {
        return m_capacity;
    }

    template <typename T>
    inline T* Dyarr<T>::data() {
        return m_data;
    }
}
