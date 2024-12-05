#include <cstdio>
#include <nautilus/dummy.hpp>

#ifndef CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_FAST_COMPILE
#endif
#include <catch2/catch_test_macros.hpp>

TEST_CASE("dummy", "[dummy]")
{
    std::vector<int> v;
    for (int n = 0; n < 15; ++n) {
        v.push_back(n);
    }
    printf("v = {");
    for (auto x : v) {
        printf("  %d", x);
    }
    printf("  }\n");
}
