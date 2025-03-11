#ifndef NAUTILUS_BITSEGMENT_HPP
#define NAUTILUS_BITSEGMENT_HPP

#include "ports-of-call/portability.hpp"

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace nautilus::tag {

// ================================================================================================

// A utility class to help operate on a subset of bits in a a larger number.  A BigSegment does not
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

public:
    // Generate a mask for the bits in the segment
    PORTABLE_FUNCTION static constexpr auto mask()
    {
        // Pick an arbitrary initial value because we first force every bit to true
        Storage mask = 0;
        mask = mask | ~mask;
        mask >>= (Nb - COUNT);
        mask <<= RSKIP;
        return mask;
    }
    // Extract the value in the segment
    PORTABLE_FUNCTION static constexpr auto get(const T t)
    {
        auto value = t & mask();
        value >>= RSKIP;
        return value;
    }
    PORTABLE_FUNCTION static constexpr auto operator()(const T t) { return get(t); }
    // Insert the value in the segment
    template <typename V>
    PORTABLE_FUNCTION static constexpr void set(const V value, T & t)
    {
        static_assert(sizeof(V) == sizeof(Storage));
        const auto masked_t = t & ~mask();
        const auto shifted_value = value << RSKIP;
        const auto masked_value = shifted_value & mask();
        assert(masked_value == shifted_value);
        t = masked_t | masked_value;
    }
    template <typename V>
    PORTABLE_FUNCTION static constexpr void operator()(const V value, T & t) {
        set(V, t)
    }
};

// ================================================================================================

} // end namespace nautilus::tag

#endif // #ifndef NAUTILUS_BITSEGMENT_HPP
