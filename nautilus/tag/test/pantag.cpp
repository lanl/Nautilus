#include "pantag.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("EntityTag", "[tag][entity_tag]")
{
    using nautilus::tag::EntityTag;

    SECTION("particle tag")
    {
        EntityTag my_tag(nautilus::tag::names::electron);
        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(!my_tag.is_nuclide());
        CHECK(my_tag.is_particle());

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_particle_index() == nautilus::tag::names::electron);
        CHECK(my_tag.get_particle_index() != nautilus::tag::names::positron);

        my_tag.set(nautilus::tag::names::positron);
        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(!my_tag.is_nuclide());
        CHECK(my_tag.is_particle());

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_particle_index() == nautilus::tag::names::positron);
        CHECK(my_tag.get_particle_index() != nautilus::tag::names::electron);
    }

    SECTION("nuclide tag (elemental)")
    {
        EntityTag my_tag(1, EntityTag::elemental);
        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_atomic_number() == 1);
        CHECK(my_tag.get_Z() == 1);

        CHECK(my_tag.is_elemental());
    }

    SECTION("nuclide tag (default index)")
    {
        EntityTag my_tag(28, 56);
        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_atomic_number() == 28);
        CHECK(my_tag.get_Z() == 28);

        CHECK(my_tag.get_atomic_mass_number() == 56);
        CHECK(my_tag.get_A() == 56);

        CHECK(my_tag.get_metastable_index() == 0);
        CHECK(my_tag.is_ground());
    }

    SECTION("nuclide tag (metastable index)")
    {
        EntityTag my_tag(6, 12, 1);
        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_atomic_number() == 6);
        CHECK(my_tag.get_Z() == 6);

        CHECK(my_tag.get_atomic_mass_number() == 12);
        CHECK(my_tag.get_A() == 12);

        CHECK(my_tag.get_metastable_index() == 1);
        CHECK(!my_tag.is_ground());
    }

    SECTION("user tag")
    {
        EntityTag my_tag(EntityTag::user, 100);

        CHECK(!my_tag.is_standard());
        CHECK(my_tag.is_user());

        CHECK(my_tag.get_user_data() == 0b00000000000000000001100100);

        CHECK(my_tag.get_version() == 0b00000);
    }
}
