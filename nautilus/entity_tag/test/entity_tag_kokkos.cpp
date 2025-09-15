#include "entity_tag.hpp"
#include "nautilus/util/gpu_check.hpp"

#include <catch2/catch_test_macros.hpp>

#include <Kokkos_Core.hpp>

#include <cstdint>

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
