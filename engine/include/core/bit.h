#pragma once

namespace nk {
    class Allocator;

    u32 leading_zeros_u32(u32 x);
    u32 trailing_zeros_u32(u32 x);
    u64 trailing_zeros_u64(u64 x);

    u32 round_up_to_power_of_2(u32 v);

    void print_binary(u64 n);
    void print_binary(u32 n);

    template <class T, int SignificantBits, int Shift = 0>
    class BitMask {
    public:
        using value_type = int;
        using iterator = BitMask;
        using const_iterator = BitMask;

        explicit BitMask(T mask) : m_mask(mask) {}

        BitMask& operator++() {
            m_mask &= (m_mask - 1);
            return *this;
        }

        explicit operator bool() const { return m_mask != 0; }
        int operator*() const { return LowestBitSet(); }

        uint32_t lowest_bit_set() const {
            return trailing_zeros_u32(m_mask) >> Shift;
        }
        uint32_t highest_bit_set() const {
            return static_cast<uint32_t>((bit_width(m_mask) - 1) >> Shift);
        }

        BitMask begin() const { return *this; }
        BitMask end() const { return BitMask(0); }

        uint32_t trailing_zeros() const { return trailing_zeros_u32(m_mask); }
        uint32_t leading_zeros() const { return leading_zeros_u32(m_mask); }

    private:
        friend bool operator==(const BitMask& a, const BitMask& b) {
            return a.m_mask == b.m_mask;
        }

        friend bool operator!=(const BitMask& a, const BitMask& b) {
            return a.m_mask != b.m_mask;
        }

        T m_mask;
    };

    inline u32 bit_mask_8(u32 bit) { return 1 << (bit & 7); }
    inline u32 bit_slot_8(u32 bit) { return bit / 8; }

    class BitSet {
    public:
        BitSet(Allocator* allocator, u32 total_bits);
        ~BitSet();

        void resize(u32 total_bits);

        void set_bit(u32 index) { bits[index / 8] |= bit_mask_8(index); }
        void clear_bit(u32 index) { bits[index / 8] &= ~bit_mask_8(index); }
        u8 get_bit(u32 index) { return bits[index / 8] & bit_mask_8(index); }

    private:
        Allocator* allocator = nullptr;
        u8* bits = nullptr;
        u32 size = 0;
    };

    template <u32 SizeInBytes>
    class BitSetFixed {
    public:
        void set_bit(u32 index) { bits[index / 8] |= bit_mask_8(index); }
        void clear_bit(u32 index) { bits[index / 8] &= ~bit_mask_8(index); }
        u8 get_bit(u32 index) { return bits[index / 8] & bit_mask_8(index); }

    private:
        u8 bits[SizeInBytes];
    };
}
