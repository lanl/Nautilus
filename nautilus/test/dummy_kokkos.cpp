#include <cstdio>
#include <nautilus/dummy_kokkos.hpp>

#ifndef CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch_test_macros.hpp>
#endif

TEST_CASE("dummy (Kokkos)", "[dummy]")
{
    printf("Hello World on Kokkos execution space %s\n", Kokkos::DefaultExecutionSpace::name());

    Kokkos::parallel_for(
        "HelloWorld", 15, KOKKOS_LAMBDA(const int i) {
            Kokkos::printf("Hello from i = %i\n", i);
        });
}
