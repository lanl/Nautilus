#ifndef NAUTILUS_BITSEGMENT_HPP
#define NAUTILUS_BITSEGMENT_HPP

#include "ports-of-call/portability.hpp"
#include "ports-of-call/robust_utils.hpp"

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace nautilus::entity_tag {

// ================================================================================================

// WARNING: This file is not part of the public user interface, and may change without warning.

// A utility class to help operate on a subset of bits in a larger number.  A BitSegment does not
// own the larger number, but only provides operations that can be applied to numbers of a
// particular type.
// -- T: The underlying storage type (the type of the "larger number")
// -- RSKIP: How many bits to the right of this segment
// -- COUNT: How many bits in this segment
// For example, T = int32_t, RSKIP = 4, COUNT = 8
//    ????????????????????XXXXXXXX????
//    <------------------><------><-->
//              I            II    III
//    I  : LSKIP  : 20 bits not included in the segment
//    II : SEGMENT: 8 bits
//    III: RSKIP  : 4 bits not included in the segment
template <typename T, size_t RSKIP, size_t COUNT>
class BitSegment
{
    // In C++23 std::bitset becomes constexpr, so it may be worth reimplementing this using bitset.
private:
    // Coerce to unsigned so shifting works the way I want
    // -- Using std::bitset may eliminate the need for this
    using Storage = typename std::make_unsigned<T>::type;
    // Number of bits in Storage
    // -- sizeof() returns a size in bytes, CHAR_BIT returns the number of bits in a char (char is
    //    defined to be one byte)
    static constexpr auto Nb = sizeof(Storage) * CHAR_BIT;
    // Ensure RSKIP fits within Storage
    static_assert(RSKIP < Nb);
    // Ensure COUNT fits within Storage
    static_assert(Nb - RSKIP >= COUNT);
    // Ensure we don't have an empty segment
    static_assert(COUNT > 0);

    // Generate the right-shifted mask: Correct width, but shifted all the way to the right
    // -- Generates the mask as the unsigned Storage type instead of the input T type in order to
    //    get logical shift-right (shift-right of signed values is implementation-defined, but
    //    often arithmetic shift-right).
    PORTABLE_FUNCTION static constexpr Storage rmask()
    {
        // Pick an arbitrary initial value because we first force every bit to true
        Storage mask = 0;
        mask = mask | ~mask;
        mask >>= (Nb - COUNT);
        return mask;
    }

public:
    // Generate a mask for the bits in the segment
    PORTABLE_FUNCTION static constexpr T mask() { return static_cast<T>(rmask() << RSKIP); }
    // Extract the value in the segment
    PORTABLE_FUNCTION static constexpr T get(const T t)
    {
        // Work in Storage instead of T because Storage is unsigned and that gets us logical
        // shift-right (fill with zero) instead of arithmetic shift-right (fill with high bit).
        Storage value = static_cast<Storage>(t);
        value >>= RSKIP;
        value &= rmask();
        return static_cast<T>(value);
    }
    // Insert the value in the segment
    template <typename V>
    PORTABLE_FUNCTION static constexpr void set(const V value, T & t)
    {
        static_assert(sizeof(V) == sizeof(Storage));
        // Prepare value (mask and shift, with sanity checks)
        const Storage value_s = static_cast<Storage>(value);
        const Storage masked_value = value_s & rmask();
        assert(masked_value == value_s);
        const Storage shifted_value = masked_value << RSKIP;
        // Prepare target (clear segment within target)
        const Storage masked_target = t & ~mask();
        // Combine value and storage
        t = masked_target | shifted_value;
    }
};

// ================================================================================================

} // end namespace nautilus::entity_tag

#endif // #ifndef NAUTILUS_BITSEGMENT_HPP
