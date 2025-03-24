#include "bitsegment.hpp"

#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

// ================================================================================================

TEST_CASE("BitSegment on GPUs", "[tag][bitsegment][GPU]")
{
    // The main tests are run on CPU.  These tests only demonstrate that the code runs on GPU.
    constexpr size_t N{6};
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;
    Kokkos::View<int *, ExecSpace> results_gpu("results", N);
    Kokkos::parallel_for(
        N, KOKKOS_LAMBDA(int const n) {
            results_gpu(n) = 0;
            using T = int32_t;
            using nautilus::tag::BitSegment;
            T num = 0x0000FFFF;
            BitSegment<T, 16, 16> bs0;
            if (bs0.mask() == static_cast<T>(0xFFFF0000)) {
                results_gpu(n) += 1;
            }
            if (bs0.get(num) == 0x0000) {
                results_gpu(n) += 2;
            }
            bs0.set(0xFFFF, num);
            if (num == static_cast<T>(0xFFFFFFFF)) {
                results_gpu(n) += 4;
            }
            BitSegment<T, 16, 10> bs1;
            if (bs1.mask() == 0x0000FFFF) {
                results_gpu(n) += 8;
            }
            if (bs1.get(num) == 0xFFFF) {
                results_gpu(n) += 16;
            }
            bs1.set(0x0000, num);
            if (num == static_cast<T>(0xFFFF0000)) {
                results_gpu(n) += 32;
            }
            printf("[%s|%d] end, diff = %d\n", ExecSpace::name(), n, 63 - results_gpu(n));
        });
    auto results_cpu = Kokkos::create_mirror_view_and_copy(HostSpace(), results_gpu);
    for (size_t n{0}; n < N; ++n) {
        CHECK(results_cpu(n) == 63);
    }
}
