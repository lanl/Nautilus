#include "bitsegment.hpp"
#include "nautilus/util/gpu_check.hpp"

#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

// ================================================================================================

TEST_CASE("BitSegment on GPUs", "[entity_tag][bitsegment][GPU]")
{
    // The main tests are run on CPU.  These tests only demonstrate that the code runs on GPU.
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;
    constexpr size_t N{6};
    GPU_CHECK_INIT();
    using T = int32_t;
    using nautilus::entity_tag::BitSegment;
    T num = 0x0000FFFF;
    BitSegment<T, 16, 16> bs0;
    GPU_CHECK(bs0.mask() == static_cast<T>(0xFFFF0000));
    GPU_CHECK(bs0.get(num) == 0x0000);
    bs0.set(0xFFFF, num);
    GPU_CHECK(num == static_cast<T>(0xFFFFFFFF));
    BitSegment<T, 0, 16> bs1;
    GPU_CHECK(bs1.mask() == 0x0000FFFF);
    GPU_CHECK(bs1.get(num) == 0xFFFF);
    bs1.set(0x0000, num);
    GPU_CHECK(num == static_cast<T>(0xFFFF0000));
    GPU_CHECK_FINAL(6);
}
