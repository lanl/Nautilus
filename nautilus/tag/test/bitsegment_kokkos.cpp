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
            using nautilus::tag::BitSegment;
            uint32_t num = 0x0000FFFF;
            BitSegment<int32_t, 16, 16> bs;
            if (bs.mask() == 0xFFFF0000) {
                results_gpu(n) += 1;
            }
            if (bs.get(num) == 0x0000) {
                results_gpu(n) += 2;
            }
            num = bs.set(0xFFFF, num);
            if (num == 0xFFFFFFFF) {
                results_gpu(n) += 4;
            }
            printf("[%s|%d] end, diff = %d\n", ExecSpace::name(), n, 7 - results_gpu(n));
        });
    auto results_cpu = Kokkos::create_mirror_view_and_copy(HostSpace(), results_gpu);
    for (size_t n{0}; n < N; ++n) {
        CHECK(results_cpu(n) == 7);
    }
}
