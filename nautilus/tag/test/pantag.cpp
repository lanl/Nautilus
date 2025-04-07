#include "pantag.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("Particle and Nuclide Tag", "[tag][pantag]")
{
    using nautilus::tag::Pantag;

    SECTION("particle tag")
    {
        Pantag my_tag(nautilus::tag::names::electron);
        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(!my_tag.is_nuclide());
        CHECK(my_tag.is_particle());

        CHECK(my_tag.get_data() == 0b00000000000000000000010000);

        CHECK(my_tag.get_version() == 0b00000);
    }

    SECTION("nuclide tag (default index)")
    {
        Pantag my_tag(28, 56);
        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        //                           +--> nuclide bit
        //                           |Z______A________MI_______
        CHECK(my_tag.get_data() == 0b10011100000111000100000000);

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_atomic_number() == 28);
        CHECK(my_tag.get_Z() == 28);

        CHECK(my_tag.get_atomic_mass_number() == 56);
        CHECK(my_tag.get_A() == 56);

        CHECK(my_tag.has_excitation_index());
        CHECK(my_tag.get_excitation_index() == 0);

        CHECK(my_tag.has_metastable_index());
        CHECK(my_tag.get_metastable_index() == 0);
    }

    SECTION("nuclide tag (excitation index)")
    {
        Pantag my_tag(2, 4, Pantag::Index::excitation, 10);
        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        //                           +--> nuclide bit
        //                           |Z______A________MI_______
        CHECK(my_tag.get_data() == 0b10000010000000100000001010);

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_atomic_number() == 2);
        CHECK(my_tag.get_Z() == 2);

        CHECK(my_tag.get_atomic_mass_number() == 4);
        CHECK(my_tag.get_A() == 4);

        CHECK(my_tag.has_excitation_index());
        CHECK(my_tag.get_excitation_index() == 10);

        CHECK(!my_tag.has_metastable_index());
    }

    SECTION("nuclide tag (metastable index)")
    {
        Pantag my_tag(6, 12, Pantag::Index::metastable, 1);
        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        //                           +--> nuclide bit
        //                           |Z______A________MI_______
        CHECK(my_tag.get_data() == 0b10000110000001100100000001);

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_atomic_number() == 6);
        CHECK(my_tag.get_Z() == 6);

        CHECK(my_tag.get_atomic_mass_number() == 12);
        CHECK(my_tag.get_A() == 12);

        CHECK(!my_tag.has_excitation_index());

        CHECK(my_tag.has_metastable_index());
        CHECK(my_tag.get_metastable_index() == 1);
    }

    SECTION("user tag")
    {
        Pantag my_tag(Pantag::user, 100);

        CHECK(!my_tag.is_standard());
        CHECK(my_tag.is_user());

        CHECK(my_tag.get_data() == 0b00000000000000000001100100);

        CHECK(my_tag.get_version() == 0b00000);
    }
}
