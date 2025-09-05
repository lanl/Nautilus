#include "names.hpp"

#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

// ================================================================================================

// TODO: Where should this go?  Very likely not here.
PORTABLE_FUNCTION constexpr bool my_strcmp(const std::string_view sv, const char * s2)
{
    const char * s1 = sv.data();
    for (; (*s1 != '\0') && (*s2 != '\0'); ++s1, ++s2) {
        if (*s1 != *s2) {
            return false;
        }
    }
    return (*s1 == '\0') && (*s2 == '\0');
};
PORTABLE_FUNCTION constexpr bool my_strcmp(const char * s1, const char * s2)
{
    for (; (*s1 != '\0') && (*s2 != '\0'); ++s1, ++s2) {
        if (*s1 != *s2) {
            return false;
        }
    }
    return (*s1 == '\0') && (*s2 == '\0');
};

// ================================================================================================

TEST_CASE("particle names on GPU", "[names][GPU]")
{
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;

    SECTION("long names")
    {
        constexpr int N{10};
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                using namespace nautilus::tag::names;
                results_g(n) = 0;
                if (my_strcmp(Particles::get_name(photon), "photon")) {
                    results_g(n) += 1;
                }
                if (my_strcmp(Particles::get_name(neutral_xi_baryon), "neutral xi baryon")) {
                    results_g(n) += 2;
                }
                if (my_strcmp(
                        Particles::get_name(
                            positive_xi_antibaryon, Particles::Standard::alternate),
                        "antiparticle of the negative xi baryon")) {
                    results_g(n) += 4;
                }
            });
        Kokkos::fence();
        auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
        for (std::size_t n = 0; n < N; ++n) {
            CHECK(results_c(n) == 7);
        }
    }

    SECTION("short names")
    {
        constexpr int N{10};
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                using namespace nautilus::tag::names;
                results_g(n) = 0;
                if (my_strcmp(Particles::get_symbol(positron), "e\u207A")) {
                    results_g(n) += 1;
                }
                if (my_strcmp(Particles::get_symbol(proton), "p")) {
                    results_g(n) += 2;
                }
                if (my_strcmp(
                        Particles::get_symbol(positive_omega_antibaryon), "\u03A9\u0304\u207A")) {
                    results_g(n) += 4;
                }
            });
        Kokkos::fence();
        auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
        for (std::size_t n = 0; n < N; ++n) {
            CHECK(results_c(n) == 7);
        }
    }
}

// ================================================================================================

TEST_CASE("nuclide names on GPU", "[names][GPU]")
{
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;

    SECTION("long names")
    {
        constexpr int N{10};
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                using namespace nautilus::tag::names;
                results_g(n) = 0;
                if (my_strcmp(Nuclides::get_name(2, Nuclides::Standard::IUPAC), "helium")) {
                    results_g(n) += 1;
                }
                if (my_strcmp(Nuclides::get_name(13, Nuclides::Standard::American), "aluminum")) {
                    results_g(n) += 2;
                }
                if (my_strcmp(Nuclides::get_name(13, Nuclides::Standard::British), "aluminium")) {
                    results_g(n) += 4;
                }
            });
        Kokkos::fence();
        auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
        for (std::size_t n = 0; n < N; ++n) {
            CHECK(results_c(n) == 7);
        }
    }

    SECTION("short names")
    {
        constexpr int N{10};
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                using namespace nautilus::tag::names;
                results_g(n) = 0;
                if (my_strcmp(Nuclides::get_symbol(1), "H")) {
                    results_g(n) += 1;
                }
                if (my_strcmp(Nuclides::get_symbol(55), "Cs")) {
                    results_g(n) += 2;
                }
                if (my_strcmp(Nuclides::get_symbol(118), "Og")) {
                    results_g(n) += 4;
                }
            });
        Kokkos::fence();
        auto results_c = Kokkos::create_mirror_view_and_copy(HostSpace(), results_g);
        for (std::size_t n = 0; n < N; ++n) {
            CHECK(results_c(n) == 7);
        }
    }
}
