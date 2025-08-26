#include "nautilus/tag/format_standard_name.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("Format: Standard Name", "[tag][format][standard name]")
{
    // "Normal" names with optional cases
    // -- trailing "g" present or absent
    // -- number after "m" present or absent
    // -- metastable vs excitation states
    // TODO: build Co-59g Pantag
    CHECK(from_standard_name("cobalt-59") == /*TODO*/);
    CHECK(from_standard_name("cobalt-59g") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "cobalt-59g");

    // TODO: build Ta-180m1 Pantag
    CHECK(from_standard_name("tantalum-180m") == /*TODO*/);
    CHECK(from_standard_name("tantalum-180m1") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "tantalum-180m1");

    // TODO: build Ta-180e1 Pantag
    CHECK(from_standard_name("tantalum-180e") == /*TODO*/);
    CHECK(from_standard_name("tantalum-180e1") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "tantalum-180e1");

    // TODO: build K-28m2 Pantag
    CHECK(from_standard_name("potassium-38m2") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "potassium-38m2");

    // TODO: build K-28e2 Pantag
    CHECK(from_standard_name("potassium-38e2") == /*TODO*/);
    CHECK(to_standard_name(Pantag(/*TODO*/)) == "potassium-38e2");

    // Alternate spellings
    // -- IUPAC
    // -- American
    // -- British
    // -- Canadian
    // TODO: build nitrogen-14 Pantag
    CHECK(to_standard_name(/*TODO*/ == nitrogen_standard);
    CHECK(to_standard_name(/*TODO*/, /*IUPAC*/) == nitrogen_standard);
    CHECK(to_standard_name(/*TODO*/, /*TODO:American*/) == nitrogen_standard);
    CHECK(to_standard_name(/*TODO*/, /*TODO:British*/) == nitrogen_standard);
    CHECK(to_standard_name(/*TODO*/, /*TODO:Canadian*/) == nitrogen_standard);

    // TODO: build aluminium-27 Pantag
    CHECK(to_standard_name(/*TODO*/) == standard_aluminium);
    CHECK(to_standard_name(/*TODO*/, /*TODO:IUPAC*/) == alternate_aluminium);
    CHECK(to_standard_name(/*TODO*/, /*TODO:American*/) == alternate_aluminium);
    CHECK(to_standard_name(/*TODO*/, /*TODO:British*/) == standard_aluminium);
    CHECK(to_standard_name(/*TODO*/, /*TODO:Canadian*/) == alternate_aluminium);

    // TODO: build sulfur-32 Pantag
    CHECK(to_standard_name(/*TODO*/) == standard_sulfur);
    CHECK(to_standard_name(/*TODO*/, /*TODO:IUPAC*/) == standard_sulfur);
    CHECK(to_standard_name(/*TODO*/, /*TODO:American*/) == standard_sulfur);
    CHECK(to_standard_name(/*TODO*/, /*TODO:British*/) == alternate_sulfur);
    CHECK(to_standard_name(/*TODO*/, /*TODO:Canadian*/) == standard_sulfur);

    // TODO: build caesium-133 Pantag
    CHECK(to_standard_name(/*TODO*/) == standard_caesium);
    CHECK(to_standard_name(/*TODO*/, /*TODO:IUPAC*/) == alternate_caesium);
    CHECK(to_standard_name(/*TODO*/, /*TODO:American*/) == alternate_caesium);
    CHECK(to_standard_name(/*TODO*/, /*TODO:British*/) == standard_caesium);
    CHECK(to_standard_name(/*TODO*/, /*TODO:Canadian*/) == standard_caesium);

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


