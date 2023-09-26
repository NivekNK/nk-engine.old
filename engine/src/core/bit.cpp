#include "nkpch.h"

#include "core/bit.h"
#include "memory/allocator.h"

namespace nk {
    u32 trailing_zeros_u32(u32 x) {
        return __builtin_ctz(x);
    }

    u32 leading_zeros_u32(u32 x) {
        return __builtin_clz(x);
    }

    u64 trailing_zeros_u64(u64 x) {
        return __builtin_ctzl(x);
    }

    u32 round_up_to_power_of_2(u32 v) {
        u32 nv = 1 << (32 - leading_zeros_u32(v));
        return nv;
    }

    void print_binary(u64 n) {
        // TODO:
        // rprint("0b");
        // for (u32 i = 0; i < 64; ++i) {
        //     u64 bit = (n >> (64 - i - 1)) & 0x1;
        //     rprint("%llu", bit);
        // }
        // rprint(" ");
    }

    void print_binary(u32 n) {
        // TODO:
        // rprint("0b");
        // for (u32 i = 0; i < 32; ++i) {
        //     u32 bit = (n >> (32 - i - 1)) & 0x1;
        //     rprint("%u", bit);
        // }
        // rprint(" ");
    }

    BitSet::BitSet(Allocator* allocator, u32 total_bits)
        : allocator{allocator}, bits{nullptr}, size{0} {
        resize(total_bits);
    }

    BitSet::~BitSet() {
        allocator->free(bits);
    }

    void BitSet::resize(u32 total_bits) {
        u8* old_bits = bits;

        const u32 new_size = (total_bits + 7) / 8;
        if (size == new_size) {
            return;
        }

        bits = allocator->allocate<u8>(new_size);

        if (old_bits) {
            memcpy(bits, old_bits, size);
            allocator->free(old_bits);
        } else {
            memset(bits, 0, new_size);
        }

        size = new_size;
    }
}
