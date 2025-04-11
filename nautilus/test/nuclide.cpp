#include "nuclide.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <algorithm>
#include <array>
#include <sstream>
#include <string>

// ================================================================================================

using Catch::Matchers::WithinRel;

// ================================================================================================

TEST_CASE("Nuclide aggregate construction", "[nuclide]")
{
    using nautilus::Nuclide;
    using nautilus::SZA;

    constexpr double mass_alpha = 6.644657230e-24;

    // Build a double-precision Nuclide
    // -- SZA, mass
    Nuclide<double> my_iso_1{SZA{2004}, mass_alpha};
    // -- int(SZA), mass
    Nuclide<double> my_iso_2(2004, mass_alpha);
    // -- S, Z, A, mass
    Nuclide<double> my_iso_3(0, 2, 4, mass_alpha);
    // -- Z, A, mass
    Nuclide<double> my_iso_4(2, 4, mass_alpha);

    // Verify data
    CHECK(my_iso_1.sza() == SZA{2004});
    CHECK(my_iso_1.mass() == mass_alpha);
    CHECK(my_iso_1.S() == 0);
    CHECK(my_iso_1.Z() == 2);
    CHECK(my_iso_1.N() == 2);
    CHECK(my_iso_1.A() == 4);

    // Verify equality
    CHECK(my_iso_1 == my_iso_2);
    CHECK(my_iso_1 == my_iso_3);
    CHECK(my_iso_1 == my_iso_4);

    // Build a single-precision Nuclide
    Nuclide<float> my_iso_sp{2004, float{mass_alpha}};

    // Verify data
    CHECK(int(my_iso_sp.sza()) == 2004);
    CHECK(my_iso_sp.mass() == float{mass_alpha});
}

// ================================================================================================

TEST_CASE("Nuclide comparison", "[nuclide]")
{
    using nautilus::Nuclide;
    using nautilus::SZA;

    constexpr double mass_he2 = 2.015894;
    constexpr double mass_he3 = 3.016029321967;
    constexpr double mass_li3 = 3.03078;

    Nuclide<double> he3{SZA{2003}, mass_he3};

    Nuclide<double> he2{SZA{2002}, mass_he2};
    Nuclide<double> he3_alt{SZA{2003}, mass_li3};
    Nuclide<double> he3m1{SZA{2, 3, 1}, mass_he3};
    Nuclide<double> li3{SZA{3003}, mass_li3};

    // Self-consistency
    CHECK(he3 == he3);
    CHECK(!(he3 != he3));
    CHECK(!(he3 < he3));
    CHECK(he3 <= he3);
    CHECK(he3 >= he3);
    CHECK(!(he3 > he3));

    // Same Z but lower N should sort before and be unequal
    CHECK(!(he2 == he3));
    CHECK(he2 != he3);
    CHECK(he2 < he3);
    CHECK(he2 <= he3);
    CHECK(!(he2 >= he3));
    CHECK(!(he2 > he3));

    // Same SZA but different mass sorts by mass
    CHECK(!(he3_alt == he3));
    CHECK(he3_alt != he3);
    CHECK(!(he3_alt < he3));
    CHECK(!(he3_alt <= he3));
    CHECK(he3_alt >= he3);
    CHECK(he3_alt > he3);

    // Nucler isomer should sort after and be unequal
    CHECK(!(he3m1 == he3));
    CHECK(he3m1 != he3);
    CHECK(!(he3m1 < he3));
    CHECK(!(he3m1 <= he3));
    CHECK(he3m1 >= he3);
    CHECK(he3m1 > he3);

    // Higher Z but same A should sort after and be unequal
    CHECK(!(li3 == he3));
    CHECK(li3 != he3);
    CHECK(!(li3 < he3));
    CHECK(!(li3 <= he3));
    CHECK(li3 >= he3);
    CHECK(li3 > he3);

    // Higher Z should sort as after even though the lower Z is a nuclear isomer (and therefore the
    // SZA, when cast to an integer, would sort the opposite)
    CHECK(!(li3 == he3m1));
    CHECK(li3 != he3m1);
    CHECK(!(li3 < he3m1));
    CHECK(!(li3 <= he3m1));
    CHECK(li3 >= he3m1);
    CHECK(li3 > he3m1);

    std::array<Nuclide, 4> nuclides{li3, he3, he3m1, he2};
    std::sort(nuclides.begin(), nuclides.end());
    std::stringstream sout;
    for (auto & nuclide : nuclides) {
        sout << nuclide << '\n';
    }
    std::string s_expected =
        "Nuclide(Z=2,A=2)\nNuclide(Z=2,A=3)\nNuclide(Z=2,A=3)\nNuclide(Z=3,A=3)\n";
    CHECK(sout.str() == s_expected);
}
