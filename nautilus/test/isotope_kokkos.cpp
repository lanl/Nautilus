#include "isotope.hpp"

#ifndef CATCH_CONFIG_FAST_COMPILE
#define CATCH_CONFIG_FAST_COMPILE
#endif
#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

// ================================================================================================

TEST_CASE("Isotope types on GPUs", "[isotope][GPU]")
{
    using DataType = double;
    constexpr size_t N{6};

    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;

    // Define data
    Kokkos::View<int *, HostSpace> Zc("Z", N);
    Kokkos::View<int *, HostSpace> Ac("A", N);
    Kokkos::View<DataType *, HostSpace> massc("mass", N);
    // note: masses from Wikipedia
    int idx = -1;
    // H-1
    ++idx;
    Zc(idx) = 1;
    Ac(idx) = 1;
    massc(idx) = 1.007825031898;
    // H-2
    ++idx;
    Zc(idx) = 1;
    Ac(idx) = 2;
    massc(idx) = 2.014101777844;
    // H-3
    ++idx;
    Zc(idx) = 1;
    Ac(idx) = 3;
    massc(idx) = 3.016049281320;
    // He-3
    ++idx;
    Zc(idx) = 2;
    Ac(idx) = 3;
    massc(idx) = 3.016029321967;
    // He-4
    ++idx;
    Zc(idx) = 2;
    Ac(idx) = 4;
    massc(idx) = 4.002603254130;
    // Li-6
    ++idx;
    Zc(idx) = 3;
    Ac(idx) = 6;
    massc(idx) = 6.0151228874;

    // --------------------------------------------------------------------------------------------

    // Test Isotope: SZA and mass
    SECTION("Isotope")
    {
        printf("Testing Isotope in execution space \"%s\".\n", ExecSpace::name());
        using Object = nautilus::Isotope<DataType>;
        // Create a vector of Isotope instance so that I can build them sequentially
        std::vector<Object> objects_v;
        for (size_t n{0}; n < N; ++n) {
            objects_v.emplace_back(Zc(n), Ac(n), massc(n));
        }
        // Create a Kokkos::View that wraps the data in the vector
        Kokkos::View<Object *, HostSpace> objects_cpu(objects_v.data(), N);
        // Create GPU mirrors
        auto Zg = Kokkos::create_mirror_view_and_copy(ExecSpace(), Zc);
        auto Ag = Kokkos::create_mirror_view_and_copy(ExecSpace(), Ac);
        auto massg = Kokkos::create_mirror_view_and_copy(ExecSpace(), massc);
        auto objects_gpu = Kokkos::create_mirror_view_and_copy(ExecSpace(), objects_cpu);
        // Create GPU results list
        Kokkos::View<int *, ExecSpace> results_gpu("results", N);
        // Launch GPU kernels to test the objects
        Kokkos::parallel_for(
            N, KOKKOS_LAMBDA(int const n) {
                printf("[%s|%d] start\n", ExecSpace::name(), n);
                results_gpu(n) = 0;
                printf("[%s|%d] Isotope::S()\n", ExecSpace::name(), n);
                if (objects_gpu(n).S() == 0) {
                    results_gpu(n) += 1;
                }
                printf("[%s|%d] Isotope::Z()\n", ExecSpace::name(), n);
                if (objects_gpu(n).Z() == Zg(n)) {
                    results_gpu(n) += 2;
                }
                printf("[%s|%d] Isotope::N()\n", ExecSpace::name(), n);
                if (objects_gpu(n).N() == Ag(n) - Zg(n)) {
                    results_gpu(n) += 4;
                }
                printf("[%s|%d] Isotope::A()\n", ExecSpace::name(), n);
                if (objects_gpu(n).A() == Ag(n)) {
                    results_gpu(n) += 8;
                }
                printf("[%s|%d] Isotope::sza()\n", ExecSpace::name(), n);
                if (objects_gpu(n).sza() == nautilus::SZA(Zg(n), Ag(n))) {
                    results_gpu(n) += 16;
                }
                printf("[%s|%d] Isotope::mass()\n", ExecSpace::name(), n);
                if (objects_gpu(n).mass() == massg(n)) {
                    results_gpu(n) += 32;
                }
                printf("[%s|%d] Isotope::operator==()\n", ExecSpace::name(), n);
                Object obj(Zg(n), Ag(n), massg(n));
                if (objects_gpu(n) == obj) {
                    results_gpu(n) += 64;
                }
                printf("[%s|%d] end, diff = %d\n", ExecSpace::name(), n, 127 - results_gpu(n));
            });
        // Copy results back to the CPU
        auto results_cpu = Kokkos::create_mirror_view_and_copy(HostSpace(), results_gpu);
        // Verify results
        for (size_t n{0}; n < N; ++n) {
            CHECK(results_cpu(n) == 127);
        }
    }
}
