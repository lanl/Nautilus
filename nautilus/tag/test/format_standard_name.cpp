#include "nautilus/tag/format_standard_name.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("Format: Standard Name", "[tag][format][standard name]")
{
    // "Normal" names with optional cases
    constexpr Pantag co59g(27, 59);
    CHECK(from_standard_name("cobalt-59") == co59g);
    CHECK(from_standard_name("cobalt-59g") == co59g);
    CHECK(to_standard_name(co59g) == "cobalt-59g");

    constexpr Pantag ta180m1(73, 180, Pantag::Index::metastable, 1);
    CHECK(from_standard_name("tantalum-180m") == ta180m1);
    CHECK(from_standard_name("tantalum-180m1") == ta180m1);
    CHECK(to_standard_name(ta180m1) == "tantalum-180m1");

    constexpr Pantag mo93e1(42, 93, Pantag::Index::excitation, 1);
    CHECK(from_standard_name("molybdenum-93e") == mo93e1);
    CHECK(from_standard_name("molybdenum-93e1") == mo93e1);
    CHECK(to_standard_name(mo93e1) == "molybdenum-93e1");

    constexpr Pantag k38m2(19, 38, Pantag::Index::metastable, 2);
    CHECK(from_standard_name("potassium-38m2") == k38m2);
    CHECK(to_standard_name(k38m2) == "potassium-38m2");

    constexpr Pantag pb188e2(82, 188, Pantag::Index::excitation, 2);
    CHECK(from_standard_name("lead-188e2") == pb188e2);
    CHECK(to_standard_name(pb188e2) == "lead-188e2");

    // Alternate spellings
    constexpr Pantag n14(7, 14);
    const std::string standard_nitrogen = "nitrogen-14";
    CHECK(to_standard_name(n14) == standard_nitrogen);
    CHECK(to_standard_name(n14, Nuclide::Standard::IUPAC) == standard_nitrogen);
    CHECK(to_standard_name(n14, Nuclide::Standard::American) == standard_nitrogen);
    CHECK(to_standard_name(n14, Nuclide::Standard::British) == standard_nitrogen);
    CHECK(to_standard_name(n14, Nuclide::Standard::Canadian) == standard_nitrogen);

    constexpr Pantag al27(13, 27);
    const std::string standard_aluminium = "aluminium-27";
    const std::string alternat_ealuminium = "aluminum-27";
    CHECK(to_standard_name(al27) == standard_aluminium);
    CHECK(to_standard_name(al27, Nuclide::Standard::IUPAC) == alternate_aluminium);
    CHECK(to_standard_name(al27, Nuclide::Standard::American) == alternate_aluminium);
    CHECK(to_standard_name(al27, Nuclide::Standard::British) == standard_aluminium);
    CHECK(to_standard_name(al27, Nuclide::Standard::Canadian) == alternate_aluminium);

    constexpr Pantag s32(16, 32);
    const std::string standard_sulfur = "sulfur-32";
    const std::string alternate_sulfur = "sulphur-32";
    CHECK(to_standard_name(s32) == standard_sulfur);
    CHECK(to_standard_name(s32, Nuclide::Standard::IUPAC) == standard_sulfur);
    CHECK(to_standard_name(s32, Nuclide::Standard::American) == standard_sulfur);
    CHECK(to_standard_name(s32, Nuclide::Standard::British) == alternate_sulfur);
    CHECK(to_standard_name(s32, Nuclide::Standard::Canadian) == standard_sulfur);

    constexpr Pantag cs55(55, 133);
    const std::string standard_caesium = "caesium-133";
    const std::string alternate_caesium = "cesium-133";
    CHECK(to_standard_name(cs55) == standard_caesium);
    CHECK(to_standard_name(cs55, Nuclide::Standard::IUPAC) == alternate_caesium);
    CHECK(to_standard_name(cs55, Nuclide::Standard::American) == alternate_caesium);
    CHECK(to_standard_name(cs55, Nuclide::Standard::British) == standard_caesium);
    CHECK(to_standard_name(cs55, Nuclide::Standard::Canadian) == standard_caesium);

    // Elementals
    constexpr Pantag c_elemental(6, Pantag::elemental);
    CHECK(to_standard_name(c_elemental) == "elemental carbon");
    constexpr Pantag cs_elemental(55, Pantag::elemental);
    CHECK(to_standard_name(cs_elemental, Nuclide::Standard::IUPAC) == "elemental caesium");
    CHECK(to_standard_name(cs_elemental, Nuclide::Standard::American) == "elemental cesium");

    // Particles
    constexpr Pantag nu_e(nautilus::tag::names::electron_neutrino);
    CHECK(to_standard_name(nu_e) == "electron neutrino");
    CHECK(to_standard_name(nu_e, Particle::Standard::PDG) == "electron neutrino");
    CHECK(to_standard_name(nu_e, Particle::Standard::alternate) == "electron neutrino");

    constexpr Pantag aL0(nautilus::tag::names::neutral_lambda_antibaryon);
    CHECK(to_standard_name(aL0) == "neutral lambda antibaryon");
    CHECK(to_standard_name(aL0, Particle::Standard::PDG) == "neutral lambda antibaryon");
    CHECK(to_standard_name(aL0, Particle::Standard::alternate) == "antiparticle of the neutral lambda baryon");

    // Distinction between hydrogen-1 (nuclide) and proton (particle)
    CHECK(to_standard_name(Pantag(nautilus::tag::names::proton) == "proton");
    CHECK(to_standard_name(Pantag(1, 1)) == "hydrogen-1");

    // First and last (verify bounds of indexing)
    constexpr Pantag h1(1, 1);
    CHECK(to_standard_name(h1) == "hydrogen-1");
    constexpr Pantag og294(118, 294);
    CHECK(to_standard_name(og294) == "oganesson-294");
    constexpr Pantag g(nautilus::tag::names::photon);
    CHECK(to_standard_name(g) == "photon");
    constexpr Pantag aOm_plus(nautilus::tag::names::positive_omega_antibaryon);
    CHECK(to_standard_name(aOm_plus) == "positive omega antibaryon");

}

// TODO: symbols


