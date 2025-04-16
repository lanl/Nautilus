#include "SZA.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SZA ctors & explicit conversion", "[SZA]")
{
    using namespace nautilus;

    SECTION("explicit single argument")
    {
        constexpr SZA he4{2004};

        CHECK(static_cast<int>(he4) == 2004);
    }

    SECTION("multi-argument (z, a, s)")
    {
        constexpr SZA pa234m1{91, 234, 1};

        CHECK(static_cast<int>(pa234m1) == 1091234);
    }
}

TEST_CASE("SZA accessors", "[SZA]")
{
    using namespace nautilus;

    SZA const he4{2, 4};
    SZA const pa234m1{91, 234, 1};

    SECTION("test s-number accessor")
    {
        CHECK(he4.S() == 0);
        CHECK(pa234m1.S() == 1);
    }

    SECTION("test z-number accessor")
    {
        CHECK(he4.Z() == 2);
        CHECK(pa234m1.Z() == 91);
    }

    SECTION("test a-number accessor")
    {
        CHECK(he4.A() == 4);
        CHECK(pa234m1.A() == 234);
    }
}

TEST_CASE("SZA equality comparison")
{
    using namespace nautilus;

    SZA const he4{2004};
    SZA const helium{he4};
    SZA const pa234m1{91, 234, 1};

    CHECK(he4 == helium);
    CHECK(not(he4 == pa234m1));
}

TEST_CASE("SZA inequality comparison")
{
    using namespace nautilus;

    SZA const he4{2004};
    SZA const he4m1{2, 4, 1};
    SZA const pa234{91, 234};
    SZA const pa234m1{91, 234, 1};

    CHECK(he4 < he4m1);
    CHECK(he4 < pa234);
    CHECK(he4 < pa234m1);

    CHECK(he4m1 < pa234);
    CHECK(he4m1 < pa234m1);

    CHECK(pa234 < pa234m1);
}
