#include "nautilus/entity_tag/format_ic.hpp"
#include "nautilus/entity_tag/entity_tag.hpp"
#include "nautilus/entity_tag/names.hpp"

#include <catch2/catch_test_macros.hpp>

// Note that the Catch2 magic doesn't always play well with Nautilus.  See
// https://stackoverflow.com/q/59770581/1791919 for a discussion of a related problem.

// ================================================================================================

TEST_CASE("format: IC chemsym", "[entity_tag][format][IC]")
{
    using nautilus::entity_tag::EntityTag;
    using nautilus::entity_tag::from_IC_chemsym;
    using nautilus::entity_tag::to_IC_chemsym;

    // Particles
    constexpr EntityTag neutron(nautilus::entity_tag::names::neutron);
    CHECK((from_IC_chemsym("nt1") == neutron));
    CHECK(to_IC_chemsym(neutron) == "nt1");

    constexpr EntityTag photon(nautilus::entity_tag::names::photon);
    CHECK((from_IC_chemsym("g0") == photon));
    CHECK((from_IC_chemsym("g") == photon));
    CHECK(to_IC_chemsym(photon) == "g0");

    // Normal nuclides
    constexpr EntityTag co59g(27, 59);
    CHECK((from_IC_chemsym("co59g") == co59g));
    CHECK((from_IC_chemsym("co59") == co59g));
    CHECK(to_IC_chemsym(co59g) == "co59");

    constexpr EntityTag ta180m1(73, 180, 1);
    CHECK((from_IC_chemsym("ta180m1") == ta180m1));
    CHECK(to_IC_chemsym(ta180m1) == "ta180m1");

    constexpr EntityTag k38m2(19, 38, 2);
    CHECK((from_IC_chemsym("k38m2") == k38m2));
    CHECK(to_IC_chemsym(k38m2) == "k38m2");

    // Special cases

    // Am-242g
    // -- Am-242g and Am-242m1 are swapped in IC chemsym
    constexpr EntityTag am242g(95, 242);
    CHECK((from_IC_chemsym("am42") == am242g));
    CHECK((from_IC_chemsym("am042") == am242g));
    CHECK((from_IC_chemsym("am242m1") == am242g));
    CHECK(to_IC_chemsym(am242g) == "am242m1");

    // Am-242m1
    // -- Am-242g and Am-242m1 are swapped in NDI SZA
    constexpr EntityTag am242m1(95, 242, 1);
    CHECK((from_IC_chemsym("am242g") == am242m1));
    CHECK((from_IC_chemsym("am242") == am242m1));
    CHECK(to_IC_chemsym(am242m1) == "am242");

    // Am-242m2
    // -- should be normal
    constexpr EntityTag am242m2(95, 242, 2);
    CHECK((from_IC_chemsym("am242m2") == am242m2));
    CHECK(to_IC_chemsym(am242m2) == "am242m2");

    // Am-243g
    // -- should be normal
    constexpr EntityTag am243g(95, 243);
    CHECK((from_IC_chemsym("am243g") == am243g));
    CHECK((from_IC_chemsym("am243") == am243g));
    CHECK(to_IC_chemsym(am243g) == "am243");

    // Am-244g
    // -- should be normal
    constexpr EntityTag am244g(95, 244);
    CHECK((from_IC_chemsym("am244g") == am244g));
    CHECK((from_IC_chemsym("am244") == am244g));
    CHECK(to_IC_chemsym(am244g) == "am244");

    // Lr-266
    // -- IC chemsym uses the outdated symbol "Lw" instead of the modern "Lr"
    constexpr EntityTag lr266(103, 266);
    CHECK((from_IC_chemsym("lw266") == lr266));
    CHECK(to_IC_chemsym(lr266) == "lw266");

    // Elementals
    constexpr EntityTag n_elemental(7, EntityTag::elemental);
    CHECK((from_IC_chemsym("n") == n_elemental));
    CHECK(to_IC_chemsym(n_elemental) == "n");
    constexpr EntityTag ca_elemental(20, EntityTag::elemental);
    CHECK((from_IC_chemsym("ca") == ca_elemental));
    CHECK(to_IC_chemsym(ca_elemental) == "ca");

    // Bad inputs
    CHECK(to_IC_chemsym(EntityTag(nautilus::entity_tag::names::electron_neutrino)) == "unknown");
    CHECK(to_IC_chemsym(EntityTag(127, 255)) == "unknown");
}
