#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

#include <cstdint>

// ================================================================================================

// This machinery will allow up to 64 calls to GPU_CHECK between init and final.  It reserves the
// names rg, rc, ectr, and ES.  Calls to init and final MUST be properly paired, and only one such
// pair can exist in a scope (consider using Catch2's SECTION to contain scopes).
//
// The fundamental concept is that GPU kernels and the `CHECK` macro don't play nice right now
// (hopefully Catch2 will provide better support for GPUs in the future).  This machinery will
// track errors by setting bits on an unsigned integer, then copy the results back to the CPU and
// check that the overall flag is correct on the CPU.
//
// By having each test activate a different bit, this is limited to only 64 tests per scope.  But
// it means that there is information that (with a little interpretation) identifies _which_ tests
// failed on the GPU.
//
// By requiring that the results are default-fail, we can distinguish between a kernel that passed
// all of its tests and a kernel that silently crashed early.  The downside is that the user must
// pass the number of tests to GPU_CHECK_FINAL in order to verify that the correct number of tests
// ran.
constexpr uint64_t error_code(const uint64_t num_tests)
{
    assert(num_tests <= 64);
    uint64_t code = 0;
    for (uint64_t i = 0; i < num_tests; ++i) {
        code += 1ULL << i;
    }
    return code;
}
#define GPU_CHECK_INIT(num_kernels)                                                               \
    using ES = Kokkos::DefaultExecutionSpace::memory_space;                                       \
    Kokkos::View<std::uint64_t *, ES> rg("results", num_kernels);                                 \
    Kokkos::parallel_for(                                                                         \
        num_kernels, KOKKOS_LAMBDA(int const n) {                                                 \
            rg(n) = 0;                                                                            \
            std::uint64_t ectr = 0;
#define GPU_CHECK(condition)                                                                      \
    if (condition) {                                                                              \
        rg(n) += ectr + 1;                                                                        \
    } else {                                                                                      \
        printf("[%s|%d] GPU check failed on line %d\n", ES::name(), n, __LINE__);                 \
    }                                                                                             \
    ectr = ectr * 2 + 1;
#define GPU_CHECK_FINAL(Ntest)                                                                    \
    printf(                                                                                       \
        "[%s|%d] end, diff = %" PRIu64 " (ectr = %" PRIu64 ", code = %" PRIu64 ")\n",             \
        ES::name(),                                                                               \
        n,                                                                                        \
        ectr - rg(n),                                                                             \
        ectr,                                                                                     \
        error_code(Ntest));                                                                       \
    }                                                                                             \
    );                                                                                            \
    auto rc = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace(), rg);                       \
    for (size_t n{0}; n < rc.size(); ++n) {                                                       \
        CHECK(rc(n) == error_code(Ntest));                                                        \
    }
