#include "nautilus/tag/format_standard_name.hpp"
#include "nautilus/tag/names.hpp"
#include "nautilus/tag/entity_tag.hpp"

#include <catch2/catch_test_macros.hpp>

// Note that the Catch2 magic doesn't always play well with Nautilus.  See
// https://stackoverflow.com/q/59770581/1791919 for a discussion of a related problem.

// ================================================================================================

TEST_CASE("format: long standard name", "[tag][format][standard name]")
{
    using nautilus::tag::from_standard_name;
    using nautilus::tag::EntityTag;
    using nautilus::tag::to_long_standard_name;
    using nautilus::tag::names::Nuclides;
    using nautilus::tag::names::Particles;

    // "Normal" names with optional cases
    constexpr EntityTag co59g(27, 59);
    CHECK((from_standard_name("cobalt-59") == co59g));
    CHECK((from_standard_name("cobalt-59g") == co59g));
    CHECK(to_long_standard_name(co59g) == "cobalt-59");

    constexpr EntityTag ta180m1(73, 180, 1);
    CHECK((from_standard_name("tantalum-180m") == ta180m1));
    CHECK((from_standard_name("tantalum-180m1") == ta180m1));
    CHECK(to_long_standard_name(ta180m1) == "tantalum-180m1");

    constexpr EntityTag k38m2(19, 38, 2);
    CHECK((from_standard_name("potassium-38m2") == k38m2));
    CHECK(to_long_standard_name(k38m2) == "potassium-38m2");

    // Alternate spellings
    constexpr EntityTag n14(7, 14);
    const std::string default_nitrogen = "nitrogen-14";
    CHECK((from_standard_name(default_nitrogen) == n14));
    CHECK(to_long_standard_name(n14) == default_nitrogen);
    CHECK(to_long_standard_name(n14, Nuclides::Standard::IUPAC) == default_nitrogen);
    CHECK(to_long_standard_name(n14, Nuclides::Standard::American) == default_nitrogen);
    CHECK(to_long_standard_name(n14, Nuclides::Standard::British) == default_nitrogen);
    CHECK(to_long_standard_name(n14, Nuclides::Standard::Canadian) == default_nitrogen);

    constexpr EntityTag al27(13, 27);
    const std::string default_aluminium = "aluminium-27";
    const std::string alternate_aluminium = "aluminum-27";
    CHECK((from_standard_name(default_aluminium) == al27));
    CHECK((from_standard_name(alternate_aluminium) == al27));
    CHECK(to_long_standard_name(al27) == default_aluminium);
    CHECK(to_long_standard_name(al27, Nuclides::Standard::IUPAC) == default_aluminium);
    CHECK(to_long_standard_name(al27, Nuclides::Standard::American) == alternate_aluminium);
    CHECK(to_long_standard_name(al27, Nuclides::Standard::British) == default_aluminium);
    CHECK(to_long_standard_name(al27, Nuclides::Standard::Canadian) == alternate_aluminium);

    constexpr EntityTag s32(16, 32);
    const std::string default_sulfur = "sulfur-32";
    const std::string alternate_sulfur = "sulphur-32";
    CHECK((from_standard_name(default_sulfur) == s32));
    CHECK((from_standard_name(alternate_sulfur) == s32));
    CHECK(to_long_standard_name(s32) == default_sulfur);
    CHECK(to_long_standard_name(s32, Nuclides::Standard::IUPAC) == default_sulfur);
    CHECK(to_long_standard_name(s32, Nuclides::Standard::American) == default_sulfur);
    CHECK(to_long_standard_name(s32, Nuclides::Standard::British) == alternate_sulfur);
    CHECK(to_long_standard_name(s32, Nuclides::Standard::Canadian) == default_sulfur);

    constexpr EntityTag cs55(55, 133);
    const std::string default_caesium = "caesium-133";
    const std::string alternate_caesium = "cesium-133";
    CHECK((from_standard_name(default_caesium) == cs55));
    CHECK((from_standard_name(alternate_caesium) == cs55));
    CHECK(to_long_standard_name(cs55) == default_caesium);
    CHECK(to_long_standard_name(cs55, Nuclides::Standard::IUPAC) == default_caesium);
    CHECK(to_long_standard_name(cs55, Nuclides::Standard::American) == alternate_caesium);
    CHECK(to_long_standard_name(cs55, Nuclides::Standard::British) == default_caesium);
    CHECK(to_long_standard_name(cs55, Nuclides::Standard::Canadian) == default_caesium);

    // Elementals
    constexpr EntityTag c_elemental(6, EntityTag::elemental);
    CHECK((from_standard_name("elemental carbon") == c_elemental));
    CHECK(to_long_standard_name(c_elemental) == "elemental carbon");
    constexpr EntityTag cs_elemental(55, EntityTag::elemental);
    CHECK((from_standard_name("elemental caesium") == cs_elemental));
    CHECK(to_long_standard_name(cs_elemental, Nuclides::Standard::IUPAC) == "elemental caesium");
    CHECK(to_long_standard_name(cs_elemental, Nuclides::Standard::American) == "elemental cesium");

    // Particles
    constexpr EntityTag nu_e(nautilus::tag::names::electron_neutrino);
    const std::string default_nu_e = "electron neutrino";
    CHECK((from_standard_name(default_nu_e) == nu_e));
    CHECK(to_long_standard_name(nu_e) == default_nu_e);
    CHECK(to_long_standard_name(nu_e, Particles::Standard::PDG) == default_nu_e);
    CHECK(to_long_standard_name(nu_e, Particles::Standard::alternate) == default_nu_e);

    constexpr EntityTag aL0(nautilus::tag::names::neutral_lambda_antibaryon);
    const std::string default_aL0 = "neutral lambda antibaryon";
    const std::string alternate_aL0 = "antiparticle of the neutral lambda baryon";
    CHECK((from_standard_name(default_aL0) == aL0));
    CHECK((from_standard_name(alternate_aL0) == aL0));
    CHECK(to_long_standard_name(aL0) == default_aL0);
    CHECK(to_long_standard_name(aL0, Particles::Standard::PDG) == default_aL0);
    CHECK(to_long_standard_name(aL0, Particles::Standard::alternate) == alternate_aL0);

    // Distinction between hydrogen-1 (nuclide) and proton (particle)
    constexpr EntityTag proton(nautilus::tag::names::proton);
    CHECK((from_standard_name("proton") == proton));
    CHECK(to_long_standard_name(proton) == "proton");
    constexpr EntityTag h1(1, 1);
    CHECK((from_standard_name("hydrogen-1") == h1));
    CHECK(to_long_standard_name(h1) == "hydrogen-1");

    // First and last (verify bounds of indexing)
    // H-1 already done above
    constexpr EntityTag og294(118, 294);
    CHECK((from_standard_name("oganesson-294") == og294));
    CHECK(to_long_standard_name(og294) == "oganesson-294");
    constexpr EntityTag g(nautilus::tag::names::photon);
    CHECK((from_standard_name("photon") == g));
    CHECK(to_long_standard_name(g) == "photon");
    constexpr EntityTag aOm_plus(nautilus::tag::names::positive_omega_antibaryon);
    CHECK((from_standard_name("positive omega antibaryon") == aOm_plus));
    CHECK((from_standard_name("positive omega antibaryon") == aOm_plus));
    CHECK((from_standard_name("antiparticle of the negative omega baryon") == aOm_plus));
    CHECK(to_long_standard_name(aOm_plus) == "positive omega antibaryon");
    CHECK(
        to_long_standard_name(aOm_plus, Particles::Standard::PDG) == "positive omega antibaryon");
    CHECK(
        to_long_standard_name(aOm_plus, Particles::Standard::alternate) ==
        "antiparticle of the negative omega baryon");

    // Bad input
    constexpr EntityTag unknown(EntityTag::unknown);
    CHECK((from_standard_name("BadInput") == unknown));         // garbage
    CHECK((from_standard_name("elemental ") == unknown));       // incomplete
    CHECK((from_standard_name("carbon elemental") == unknown)); // should be "elemental carbon"
    CHECK((from_standard_name("elemental steel") == unknown));  // steel isn't a nuclide
    CHECK((from_standard_name("brass-") == unknown));           // brass isn't a nuclide
    CHECK((from_standard_name("nickel-") == unknown));          // incomplete
    CHECK((from_standard_name("carbon-12z1") == unknown));      // invalid metastable indicator

    // "Bad" tag
    CHECK(to_long_standard_name(EntityTag(0, 0)) == "unknown");               // Z = 0
    CHECK(to_long_standard_name(EntityTag(127, 0)) == "unknown");             // Z > Oganesson
    CHECK(to_long_standard_name(EntityTag(EntityTag::user, 0)) == "unknown"); // user tag
}

// ================================================================================================

TEST_CASE("format: short standard name", "[tag][format][standard name]")
{
    using nautilus::tag::from_standard_name;
    using nautilus::tag::EntityTag;
    using nautilus::tag::to_short_standard_name;
    using nautilus::tag::names::Nuclides;
    using nautilus::tag::names::Particles;

    // "Normal" names with optional cases
    constexpr EntityTag co59g(27, 59);
    CHECK((from_standard_name("Co-59") == co59g));
    CHECK((from_standard_name("Co-59g") == co59g));
    CHECK(to_short_standard_name(co59g) == "Co-59");

    constexpr EntityTag ta180m1(73, 180, 1);
    CHECK((from_standard_name("Ta-180m") == ta180m1));
    CHECK((from_standard_name("Ta-180m1") == ta180m1));
    CHECK(to_short_standard_name(ta180m1) == "Ta-180m1");

    constexpr EntityTag k38m2(19, 38, 2);
    CHECK((from_standard_name("K-38m2") == k38m2));
    CHECK(to_short_standard_name(k38m2) == "K-38m2");

    // "short" standard name does not have alternate spellings

    // Elementals
    constexpr EntityTag c_elemental(6, EntityTag::elemental);
    CHECK((from_standard_name("C") == c_elemental));
    CHECK(to_short_standard_name(c_elemental) == "C");
    constexpr EntityTag cs_elemental(55, EntityTag::elemental);
    CHECK((from_standard_name("Cs") == cs_elemental));
    CHECK(to_short_standard_name(cs_elemental) == "Cs");

    // Particles
    constexpr EntityTag nu_e(nautilus::tag::names::electron_neutrino);
    const std::string default_nu_e = "\u03BD\u2091";
    CHECK((from_standard_name(default_nu_e) == nu_e));
    CHECK(to_short_standard_name(nu_e) == default_nu_e);

    constexpr EntityTag aL0(nautilus::tag::names::neutral_lambda_antibaryon);
    const std::string default_aL0 = "\u039B\u0304\u2070";
    CHECK((from_standard_name(default_aL0) == aL0));
    CHECK(to_short_standard_name(aL0) == default_aL0);

    // Distinction between hydrogen-1 (nuclide) and proton (particle)
    constexpr EntityTag proton(nautilus::tag::names::proton);
    CHECK((from_standard_name("p") == proton));
    CHECK(to_short_standard_name(proton) == "p");
    constexpr EntityTag h1(1, 1);
    CHECK((from_standard_name("H-1") == h1));
    CHECK(to_short_standard_name(h1) == "H-1");

    // Distinction between similar:
    // -- p (proton) and P (phosphorus elemental); proton already checked above
    constexpr EntityTag p_elemental(15, EntityTag::elemental);
    CHECK((from_standard_name("P") == p_elemental));
    CHECK(to_short_standard_name(p_elemental) == "P");
    // -- n (neutron) and N (nitrogen elemental)
    constexpr EntityTag neutron(nautilus::tag::names::neutron);
    constexpr EntityTag n_elemental(7, EntityTag::elemental);
    CHECK((from_standard_name("n") == neutron));
    CHECK(to_short_standard_name(neutron) == "n");
    CHECK((from_standard_name("N") == n_elemental));
    CHECK(to_short_standard_name(n_elemental) == "N");

    // First and last (verify bounds of indexing)
    // H-1 already done above
    constexpr EntityTag og294(118, 294);
    CHECK((from_standard_name("Og-294") == og294));
    CHECK(to_short_standard_name(og294) == "Og-294");
    constexpr EntityTag g(nautilus::tag::names::photon);
    CHECK((from_standard_name("\u03B3") == g));
    CHECK(to_short_standard_name(g) == "\u03B3");
    constexpr EntityTag aOm_plus(nautilus::tag::names::positive_omega_antibaryon);
    CHECK((from_standard_name("\u03A9\u0304\u207A") == aOm_plus));
    CHECK(to_short_standard_name(aOm_plus) == "\u03A9\u0304\u207A");

    // Bad input
    constexpr EntityTag unknown(EntityTag::unknown);
    CHECK((from_standard_name("BadInput") == unknown)); // garbage
    CHECK((from_standard_name("Xx-") == unknown));      // Xx isn't a nuclide
    CHECK((from_standard_name("Ni-") == unknown));      // incomplete
    CHECK((from_standard_name("Au-197q1") == unknown)); // invalid metastable state indicator

    // "Bad" tag
    CHECK(to_long_standard_name(EntityTag(0, 0)) == "unknown");               // Z = 0
    CHECK(to_long_standard_name(EntityTag(127, 0)) == "unknown");             // Z > Oganesson
    CHECK(to_long_standard_name(EntityTag(EntityTag::user, 0)) == "unknown"); // user tag
}
