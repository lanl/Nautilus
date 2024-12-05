#include "SZA.hpp"

#ifndef CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_FAST_COMPILE
#endif
#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

TEST_CASE("SZA ctors & explicit conversion on GPUs", "[SZA][GPU]")
{
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;

    SECTION("explicit single argument")
    {
        constexpr int N{10};
        constexpr nautilus::SZA he4{2004};

        // Create Kokkos Views to pass CPU variables to GPU
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        // Create parallel loop
        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                results_g(n) = 0;
                if (static_cast<int>(he4) == 2004) {
                    results_g(n) += 2;
                }
            });
        Kokkos::fence();
        // Pull back vars from CPU to GPU
        auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
        // Test that it worked
        for (size_t i{0}; i < N; ++i) {
            CHECK(results_c(i) == 2);
        }
    }

    SECTION("multi-argument (z, a, s)")
    {
        constexpr int N{10};
        constexpr nautilus::SZA pa234m1{91, 234, 1};

        // Create Kokkos Views to pass CPU variables to GPU
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        // Create parallel loop
        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                results_g(n) = 0;
                if (static_cast<int>(pa234m1) == 1091234) {
                    results_g(n) += 2;
                }
            });
        Kokkos::fence();
        // Pull back vars from CPU to GPU
        auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
        // Test that it worked
        for (size_t i{0}; i < N; ++i) {
            CHECK(results_c(i) == 2);
        }
    }
}

TEST_CASE("SZA accessors on GPUs", "[SZA][GPU]")
{
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;

    constexpr nautilus::SZA he4{2, 4};
    constexpr nautilus::SZA pa234m1{91, 234, 1};

    SECTION("test s-number accessor")
    {
        constexpr int N{10};

        // Create Kokkos Views to pass CPU variables to GPU
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        // Create parallel loop
        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                results_g(n) = 0;
                if (he4.S() == 0) {
                    results_g(n) += 2;
                }
                if (pa234m1.S() == 1) {
                    results_g(n) += 4;
                }
            });
        Kokkos::fence();
        // Pull back vars from CPU to GPU
        auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
        // Test that it worked
        for (size_t i{0}; i < N; ++i) {
            CHECK(results_c(i) == 6);
        }
    }

    SECTION("test z-number accessor")
    {
        constexpr int N{10};

        // Create Kokkos Views to pass CPU variables to GPU
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        // Create parallel loop
        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                results_g(n) = 0;
                if (he4.Z() == 2) {
                    results_g(n) += 2;
                }
                if (pa234m1.Z() == 91) {
                    results_g(n) += 4;
                }
            });
        Kokkos::fence();
        // Pull back vars from CPU to GPU
        auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
        // Test that it worked
        for (size_t i{0}; i < N; ++i) {
            CHECK(results_c(i) == 6);
        }
    }

    SECTION("test a-number accessor")
    {
        constexpr int N{10};

        // Create Kokkos Views to pass CPU variables to GPU
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        // Create parallel loop
        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                results_g(n) = 0;
                if (he4.A() == 4) {
                    results_g(n) += 2;
                }
                if (pa234m1.A() == 234) {
                    results_g(n) += 4;
                }
            });
        Kokkos::fence();
        // Pull back vars from CPU to GPU
        auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
        // Test that it worked
        for (size_t i{0}; i < N; ++i) {
            CHECK(results_c(i) == 6);
        }
    }
}

TEST_CASE("SZA equality comparison on GPUs")
{
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;

    constexpr nautilus::SZA he4{2004};
    constexpr nautilus::SZA helium{he4};
    constexpr nautilus::SZA pa234m1{91, 234, 1};

    constexpr int N{10};

    // Create Kokkos Views to pass CPU variables to GPU
    Kokkos::View<int *, ExecSpace> results_g("results", N);

    // Create parallel loop
    Kokkos::parallel_for(
        N, KOKKOS_LAMBDA(int n) {
            results_g(n) = 0;
            if (he4 == helium) {
                results_g(n) += 2;
            }
            if (he4 != pa234m1) {
                results_g(n) += 4;
            }
        });
    Kokkos::fence();
    // Pull back vars from CPU to GPU
    auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
    // Test that it worked
    for (size_t i{0}; i < N; ++i) {
        CHECK(results_c(i) == 6);
    }
}

TEST_CASE("SZA inequality comparison on GPUs")
{
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;

    constexpr nautilus::SZA he4{2004};
    constexpr nautilus::SZA he4m1{2, 4, 1};
    constexpr nautilus::SZA pa234{91, 234};
    constexpr nautilus::SZA pa234m1{91, 234, 1};

    constexpr int N{10};

    // Create Kokkos Views to pass CPU variables to GPU
    Kokkos::View<int *, ExecSpace> results_g("results", N);

    // Create parallel loop
    Kokkos::parallel_for(
        N, KOKKOS_LAMBDA(int n) {
            results_g(n) = 0;
            // Test he4
            if (he4 < he4m1) {
                results_g(n) += 2;
            }
            if (he4 < pa234) {
                results_g(n) += 4;
            }
            if (he4 < pa234m1) {
                results_g(n) += 16;
            }
            // Test he4m1
            if (he4m1 < pa234) {
                results_g(n) += 32;
            }
            if (he4m1 < pa234m1) {
                results_g(n) += 64;
            }
            // Test pa
            if (pa234 < pa234m1) {
                results_g(n) += 128;
            }
        });
    Kokkos::fence();
    // Pull back vars from CPU to GPU
    auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
    // Test that it worked
    CHECK(he4m1 < pa234);
    for (size_t i{0}; i < N; ++i) {
        CHECK(results_c(i) == 246);
    }
}
