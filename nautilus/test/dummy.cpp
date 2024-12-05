#include <cstdio>
#include <nautilus/dummy.hpp>

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
