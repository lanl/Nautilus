#include "nautilus/tag/format_standard_name.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("Format: Standard Name", "[tag][format][standard name]")
{
    // "Normal" names with optional cases
    // -- trailing "g" present or absent
    // -- number after "m" present or absent
    // -- metastable vs excitation states
    CHECK(from_standard_name("cobalt-59") == /*TODO*/);
    CHECK(from_standard_name("cobalt-59g") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "cobalt-59g");

    CHECK(from_standard_name("tantalum-180m") == /*TODO*/);
    CHECK(from_standard_name("tantalum-180m1") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "tantalum-180m1");

    CHECK(from_standard_name("tantalum-180e") == /*TODO*/);
    CHECK(from_standard_name("tantalum-180e1") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "tantalum-180e1");

    CHECK(from_standard_name("potassium-38m2") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "potassium-38m2");

    CHECK(from_standard_name("potassium-38e2") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "potassium-38e2");

    // Alternate spellings
    // -- IUPAC
    // -- American
    // -- British
    // -- Canadian
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "nitrogen-14");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*IUPAC*/) == "nitrogen-14");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:American*/) == "nitrogen-14");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:British*/) == "nitrogen-14");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:Canadian*/) == "nitrogen-14");

    CHECK(to_standard_name(Pantag(/*TODO*/)) == "aluminium-27");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:American*/) == "aluminum-27");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:British*/) == "aluminium-27");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:Canadian*/) == "aluminum-27");

    CHECK(to_standard_name(Pantag(/*TODO*/)) == "sulfur-32");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:American*/) == "sulfur-32");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:British*/) == "sulphur-32");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:Canadian*/) == "sulfur-32");

    CHECK(to_standard_name(Pantag(/*TODO*/)) == "caesium-133");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:American*/) == "cesium-133");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:British*/) == "caesium-133");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:Canadian*/) == "caesium-133");

    // Elementals
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "elemental carbon");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:American*/) == "elemental cesium");

    // Particles
    // -- PDG
    // -- alternate
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "electron neutrino");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:PDG*/) == "electron neutrino");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:alternate*/) == "electron neutrino");

    CHECK(to_standard_name(Pantag(/*TODO*/)) == "neutral lambda antibaryon");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:PDG*/) == "neutral lambda antibaryon");
    CHECK(to_standard_name(Pantag(/*TODO*/), /*TODO:alternate*/) == "antiparticle of the neutral lambda baryon");

    // Distinction between hydrogen-1 (nuclide) and proton (particle)
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "proton");
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "hydrogen-1");

    // First and last (verify bounds of indexing)
    CHECK(to_standard_name(/*TODO*/) == "hydrogen-1");
    CHECK(to_standard_name(/*TODO*/) == "oganesson-294");
    CHECK(to_standard_name(/*TODO*/) == "photon");
    CHECK(to_standard_name(/*TODO*/) == "positive omega antibaryon");

}

// TODO: symbols


