#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("basic test", "[general]")
{
    // This doesn't do anything other than confirm that the test suite is working.  But it also
    // means that if the other "general" tests are turned off, we don't get a "no sources given to
    // target" error from CMake.
    CHECK(true);
}
