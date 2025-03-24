#include "pantag.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("Particle and Nuclide Tag", "[tag][pantag]")
{
    using nautilus::tag::Pantag;

    SECTION("standard particle")
    {
        Pantag my_tag(Pantag::PNType::particle, Pantag::Mode::standard, 1);
        CHECK(!my_tag.is_nuclide());
        CHECK(my_tag.is_particle());

        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        CHECK(my_tag.get_data() == 0b0000000000000000000000001);

        CHECK(my_tag.get_version() == 0b00000);
    }

    SECTION("user particle")
    {
        Pantag my_tag(Pantag::PNType::particle, Pantag::Mode::user, 10);
        CHECK(!my_tag.is_nuclide());
        CHECK(my_tag.is_particle());

        CHECK(!my_tag.is_standard());
        CHECK(my_tag.is_user());

        CHECK(my_tag.get_data() == 0b0000000000000000000001010);

        CHECK(my_tag.get_version() == 0b00000);
    }

    SECTION("standard nuclide (default index)")
    {
        Pantag my_tag(Pantag::PNType::nuclide, Pantag::Mode::standard, 28, 56);
        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        //                           Z______A________MI_______
        CHECK(my_tag.get_data() == 0b0011100000111000100000000);

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

    SECTION("standard nuclide (excitation index)")
    {
        Pantag my_tag(
            Pantag::PNType::nuclide, Pantag::Mode::standard, 2, 4, Pantag::Index::excitation, 10);
        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        //                           Z______A________MI_______
        CHECK(my_tag.get_data() == 0b0000010000000100000001010);

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_atomic_number() == 2);
        CHECK(my_tag.get_Z() == 2);

        CHECK(my_tag.get_atomic_mass_number() == 4);
        CHECK(my_tag.get_A() == 4);

        CHECK(my_tag.has_excitation_index());
        CHECK(my_tag.get_excitation_index() == 10);

        CHECK(!my_tag.has_metastable_index());
    }

    SECTION("standard nuclide(metastable index)")
    {
        Pantag my_tag(
            Pantag::PNType::nuclide, Pantag::Mode::standard, 6, 12, Pantag::Index::metastable, 1);
        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        CHECK(my_tag.is_standard());
        CHECK(!my_tag.is_user());

        //                           Z______A________MI_______
        CHECK(my_tag.get_data() == 0b0000110000001100100000001);

        CHECK(my_tag.get_version() == 0b00000);

        CHECK(my_tag.get_atomic_number() == 6);
        CHECK(my_tag.get_Z() == 6);

        CHECK(my_tag.get_atomic_mass_number() == 12);
        CHECK(my_tag.get_A() == 12);

        CHECK(!my_tag.has_excitation_index());

        CHECK(my_tag.has_metastable_index());
        CHECK(my_tag.get_metastable_index() == 1);
    }

    SECTION("user nuclide")
    {
        Pantag my_tag(Pantag::PNType::nuclide, Pantag::Mode::user, 100);
        CHECK(my_tag.is_nuclide());
        CHECK(!my_tag.is_particle());

        CHECK(!my_tag.is_standard());
        CHECK(my_tag.is_user());

        CHECK(my_tag.get_data() == 0b0000000000000000001100100);

        CHECK(my_tag.get_version() == 0b00000);
    }
}
