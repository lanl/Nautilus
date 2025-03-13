#include "bitsegment.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("BitSegment operations", "[tag][bitsegment]")
{
    using nautilus::tag::BitSegment;

    int32_t num = 0b10000000010100000000101000000001;

    BitSegment<int32_t, 0, 10> bs0;
    CHECK(bs0.mask() == 0b00000000000000000000001111111111);
    CHECK(bs0.get(num) == 0b1000000001);

    bs0.set(0b1111111111, num);
    CHECK(num == 0b10000000010100000000101111111111);

    BitSegment<int32_t, 11, 10> bs1;
    CHECK(bs1.mask() == 0b00000000000111111111100000000000);
    CHECK(bs1.get(num) == 0b1000000001);

    bs1.set(0b1111111111, num);
    CHECK(num == 0b10000000010111111111101111111111);

    // Important to test with a segment that goes all the way to the leftmost bit, and with a value
    // that uses that leftmost bit, because that catches some corner cases.
    BitSegment<int32_t, 22, 10> bs2;
    CHECK(bs2.mask() == 0b11111111110000000000000000000000);
    CHECK(bs2.get(num) == 0b1000000001);

    bs2.set(0b1111111111, num);
    CHECK(num == 0b11111111110111111111101111111111);
}
