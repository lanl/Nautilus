#include "names.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("Name indices", "[names]")
{
    CHECK(nautilus::Index::photon == 0);
}

