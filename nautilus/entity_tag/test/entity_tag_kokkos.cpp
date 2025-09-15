#include "entity_tag.hpp"

#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

#include <cstdint>

// ================================================================================================

// TODO: Put this somewhere more generic so that other tests can also use it.
// This machinery will allow up to 64 calls to GPU_CHECK between init and final.  It reserves the
// names rg, rc, and ectr.  Calls to init and final MUST be properly paired, and only one such pair
// can exist in a scope (consider using Catch2's SECTION to contain scopes).
constexpr uint64_t error_code(const uint64_t num_tests)
{
    uint64_t code = 0;
    for (uint64_t i = 0; i < num_tests; ++i) {
        code += 1 << i;
    }
    return code;
}
#define GPU_CHECK_INIT()                                                                          \
    Kokkos::View<std::uint64_t *, ExecSpace> rg("results", N);                                    \
    Kokkos::parallel_for(                                                                         \
        N, KOKKOS_LAMBDA(int const n) {                                                           \
            rg(n) = 0;                                                                            \
            std::uint64_t ectr = 0;
#define GPU_CHECK(condition)                                                                      \
    if (condition) {                                                                              \
        rg(n) += ectr + 1;                                                                        \
    } else {                                                                                      \
        printf("[%s|%d] GPU check failed on line %d\n",                                           \
            ExecSpace::name(), n, __LINE__);                                                      \
    }                                                                                             \
    ectr = ectr * 2 + 1;
#define GPU_CHECK_FINAL(Ntest)                                                                    \
            printf("[%s|%d] end, diff = %" PRIu64 " (ectr = %" PRIu64 ", code = %" PRIu64 ")\n",  \
                ExecSpace::name(), n, ectr - rg(n),                                               \
                ectr,                                                                             \
                error_code(Ntest));                                                               \
        }                                                                                         \
    );                                                                                            \
    auto rc = Kokkos::create_mirror_view_and_copy(HostSpace(), rg);                               \
    for (size_t n{0}; n < N; ++n) {                                                               \
        CHECK(rc(n) == error_code(Ntest));                                                        \
    }

// ================================================================================================

TEST_CASE("EntityTag on GPUs", "[entity_tag][GPU]")
{
    using nautilus::entity_tag::EntityTag;
    using HostSpace = Kokkos::HostSpace;
    using ExecSpace = Kokkos::DefaultExecutionSpace::memory_space;
    constexpr size_t N{6};

    SECTION("particle tag")
    {
        GPU_CHECK_INIT();

        EntityTag my_tag(nautilus::entity_tag::names::electron);

        GPU_CHECK(my_tag.is_standard());
        GPU_CHECK(!my_tag.is_user());
        GPU_CHECK(!my_tag.is_nuclide());
        GPU_CHECK(my_tag.is_particle());
        GPU_CHECK(my_tag.get_particle_index() == nautilus::entity_tag::names::electron);
        GPU_CHECK(my_tag.get_particle_index() != nautilus::entity_tag::names::positron);

        my_tag.set(nautilus::entity_tag::names::positron);

        GPU_CHECK(my_tag.is_standard());
        GPU_CHECK(!my_tag.is_user());
        GPU_CHECK(!my_tag.is_nuclide());
        GPU_CHECK(my_tag.is_particle());
        GPU_CHECK(my_tag.get_version() == 0b00000);
        GPU_CHECK(my_tag.get_particle_index() == nautilus::entity_tag::names::positron);
        GPU_CHECK(my_tag.get_particle_index() != nautilus::entity_tag::names::electron);

        GPU_CHECK_FINAL(13);
    }

    SECTION("nuclide tag (elemental)")
    {
        GPU_CHECK_INIT();

        constexpr EntityTag my_tag(1, EntityTag::elemental);

        GPU_CHECK(my_tag.is_standard());
        GPU_CHECK(!my_tag.is_user());
        GPU_CHECK(my_tag.is_nuclide());
        GPU_CHECK(!my_tag.is_particle());
        GPU_CHECK(my_tag.get_version() == 0b00000);
        GPU_CHECK(my_tag.get_atomic_number() == 1);
        GPU_CHECK(my_tag.get_Z() == 1);
        GPU_CHECK(my_tag.is_elemental());

        GPU_CHECK_FINAL(8);
    }

    SECTION("nuclide tag (default index)")
    {
        GPU_CHECK_INIT();

        constexpr EntityTag my_tag(28, 56);

        GPU_CHECK(my_tag.is_standard());
        GPU_CHECK(!my_tag.is_user());
        GPU_CHECK(my_tag.is_nuclide());
        GPU_CHECK(!my_tag.is_particle());
        GPU_CHECK(my_tag.get_version() == 0b00000);
        GPU_CHECK(my_tag.get_atomic_number() == 28);
        GPU_CHECK(my_tag.get_Z() == 28);
        GPU_CHECK(my_tag.get_atomic_mass_number() == 56);
        GPU_CHECK(my_tag.get_A() == 56);
        GPU_CHECK(!my_tag.is_elemental());
        GPU_CHECK(my_tag.is_ground());
        GPU_CHECK(my_tag.get_metastable_index() == 0);

        GPU_CHECK_FINAL(12);
    }

    SECTION("nuclide tag (metastable index)")
    {
        GPU_CHECK_INIT();

        constexpr EntityTag my_tag(6, 12, 1);

        GPU_CHECK(my_tag.is_standard());
        GPU_CHECK(!my_tag.is_user());
        GPU_CHECK(my_tag.is_nuclide());
        GPU_CHECK(!my_tag.is_particle());
        GPU_CHECK(my_tag.get_version() == 0b00000);
        GPU_CHECK(my_tag.get_atomic_number() == 6);
        GPU_CHECK(my_tag.get_Z() == 6);
        GPU_CHECK(my_tag.get_atomic_mass_number() == 12);
        GPU_CHECK(my_tag.get_A() == 12);
        GPU_CHECK(!my_tag.is_elemental());
        GPU_CHECK(!my_tag.is_ground());
        GPU_CHECK(my_tag.get_metastable_index() == 1);

        GPU_CHECK_FINAL(12);
    }

    SECTION("user tag")
    {
        GPU_CHECK_INIT();

        constexpr EntityTag my_tag(EntityTag::user, 100);

        GPU_CHECK(!my_tag.is_standard());
        GPU_CHECK(my_tag.is_user());
        GPU_CHECK(my_tag.get_version() == 0b00000);
        GPU_CHECK(my_tag.get_user_data() == 0b00000000000000000001100100);

        GPU_CHECK_FINAL(4);
    }
}
