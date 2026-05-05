#include "names.hpp"

#include "nautilus/util/string_processing.hpp"

#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

// ================================================================================================

TEST_CASE("particle names on GPU", "[names][GPU]")
{
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;

    using nautilus::entity_tag::my_strcmp;

    SECTION("long names")
    {
        constexpr int N{10};
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                using namespace nautilus::entity_tag::names;
                results_g(n) = 0;
                if (my_strcmp(Particles::get_name(photon), "photon")) {
                    results_g(n) += 1;
                }
                if (my_strcmp(Particles::get_name(neutral_xi_baryon), "neutral xi baryon")) {
                    results_g(n) += 2;
                }
                if (my_strcmp(
                        Particles::get_name(positive_xi_antibaryon, Particles::Standard::textbook),
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
                using namespace nautilus::entity_tag::names;
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

    using nautilus::entity_tag::my_strcmp;

    SECTION("long names")
    {
        constexpr int N{10};
        Kokkos::View<int *, ExecSpace> results_g("results", N);

        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int n) {
                using namespace nautilus::entity_tag::names;
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
                using namespace nautilus::entity_tag::names;
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
