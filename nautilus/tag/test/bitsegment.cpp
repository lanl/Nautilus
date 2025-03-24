#include "bitsegment.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("BitSegment operations", "[tag][bitsegment]")
{
    using T = int32_t;
    using nautilus::tag::BitSegment;

    T num = 0b10000000010100000000101000000001;

    BitSegment<T, 0, 10> bs0;
    CHECK(bs0.mask() == static_cast<T>(0b00000000000000000000001111111111));
    CHECK(bs0.get(num) == static_cast<T>(0b1000000001));

    bs0.set(0b1111111111, num);
    CHECK(num == static_cast<T>(0b10000000010100000000101111111111));

    BitSegment<T, 11, 10> bs1;
    CHECK(bs1.mask() == static_cast<T>(0b00000000000111111111100000000000));
    CHECK(bs1.get(num) == static_cast<T>(0b1000000001));

    bs1.set(0b1111111111, num);
    CHECK(num == static_cast<T>(0b10000000010111111111101111111111));

    // Important to test with a segment that goes all the way to the leftmost bit, and with a value
    // that uses that leftmost bit, because that catches some corner cases.
    BitSegment<T, 22, 10> bs2;
    CHECK(bs2.mask() == static_cast<T>(0b11111111110000000000000000000000));
    CHECK(bs2.get(num) == static_cast<T>(0b1000000001));

    bs2.set(0b1111111111, num);
    CHECK(num == static_cast<T>(0b11111111110111111111101111111111));
}
