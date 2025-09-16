#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

#include <cstdint>

// ================================================================================================

// This machinery will allow up to 64 calls to GPU_CHECK between init and final.  It reserves the
// names rg, rc, ectr, and ES.  Calls to init and final MUST be properly paired, and only one such
// pair can exist in a scope (consider using Catch2's SECTION to contain scopes).
constexpr uint64_t error_code(const uint64_t num_tests)
{
    uint64_t code = 0;
    for (uint64_t i = 0; i < num_tests; ++i) {
        code += 1 << i;
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
