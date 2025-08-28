#include "nautilus/tag/format_ndi.hpp"
#include "nautilus/tag/names.hpp"
#include "nautilus/tag/pantag.hpp"

#include <catch2/catch_test_macros.hpp>

// Note that the Catch2 magic doesn't always play well with Nautilus.  See
// https://stackoverflow.com/q/59770581/1791919 for a discussion of a related problem.

// ================================================================================================

TEST_CASE("format: NDI SZA", "[tag][format][NDI]")
{
    using nautilus::tag::from_NDI_SZA;
    using nautilus::tag::Pantag;
    using nautilus::tag::to_NDI_SZA;

    // Particles
    constexpr Pantag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_SZA(1) == neutron));
    CHECK(to_NDI_SZA(neutron) == 1);

    constexpr Pantag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_SZA(0) == photon));
    CHECK(to_NDI_SZA(photon) == 0);

    // Normal nuclides
    constexpr Pantag co59g(27, 59);
    CHECK((from_NDI_SZA(27059) == co59g));
    CHECK(to_NDI_SZA(co59g) == 27059);

    constexpr Pantag ta180m1(73, 180, Pantag::Index::metastable, 1);
    CHECK((from_NDI_SZA(1073180) == ta180m1));
    CHECK(to_NDI_SZA(ta180m1) == 1073180);

    constexpr Pantag k38m2(19, 38, Pantag::Index::metastable, 2);
    CHECK((from_NDI_SZA(2019038) == k38m2));
    CHECK(to_NDI_SZA(k38m2) == 2019038);

    // Special cases

    // Am-242g
    // -- Am-242g and Am-242m1 are swapped in NDI SZA
    // -- Am-242g can take different values
    //    -- 1095242 (most cases)
    //    -- 95042 (specific cases)
    //       -- mendf70x / 701nm
    //       -- mtmg01 / 121nm - 135nm
    //       -- mtmg01ex / 121nm - 135nm
    constexpr Pantag am242g(95, 242);
    constexpr int default_am242g = 1095242;
    constexpr int alternate_am242g = 95042;
    CHECK((from_NDI_SZA(default_am242g) == am242g));
    CHECK((from_NDI_SZA(alternate_am242g) == am242g));
    CHECK(to_NDI_SZA(am242g) == default_am242g);
    CHECK(to_NDI_SZA(am242g, "mtmg08") == default_am242g);
    CHECK(to_NDI_SZA(am242g, "083nm") == default_am242g);
    CHECK(to_NDI_SZA(am242g, "083") == default_am242g);
    CHECK(to_NDI_SZA(am242g, 83) == default_am242g);
    CHECK(to_NDI_SZA(am242g, 0.083) == default_am242g);
    CHECK(to_NDI_SZA(am242g, "mendf70x") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "701nm") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "701") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, 701) == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, 0.701) == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "mtmg01") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "mtmg01ex") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "121nm") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "128nm") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "135nm") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "121") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "128") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, "135") == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, 121) == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, 128) == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, 135) == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, 0.121) == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, 0.128) == alternate_am242g);
    CHECK(to_NDI_SZA(am242g, 0.135) == alternate_am242g);

    // Am-242m1
    // -- Am-242g and Am-242m1 are swapped in NDI SZA
    constexpr Pantag am242m1(95, 242, Pantag::Index::metastable, 1);
    CHECK((from_NDI_SZA(95242) == am242m1));
    CHECK(to_NDI_SZA(am242m1) == 95242);

    // Am-242m2
    // -- should be normal
    constexpr Pantag am242m2(95, 242, Pantag::Index::metastable, 2);
    CHECK((from_NDI_SZA(2095242) == am242m2));
    CHECK(to_NDI_SZA(am242m2) == 2095242);

    // Am-243g
    // -- should be normal
    constexpr Pantag am243g(95, 243);
    CHECK((from_NDI_SZA(95243) == am243g));
    CHECK(to_NDI_SZA(am243g) == 95243);

    // Am-243m1
    // -- should be normal
    constexpr Pantag am243m1(95, 243, Pantag::Index::metastable, 1);
    CHECK((from_NDI_SZA(1095243) == am243m1));
    CHECK(to_NDI_SZA(am243m1) == 1095243);

    // Am-243m2
    // -- should be normal
    constexpr Pantag am243m2(95, 243, Pantag::Index::metastable, 2);
    CHECK((from_NDI_SZA(2095243) == am243m2));
    CHECK(to_NDI_SZA(am243m2) == 2095243);

    // Am-244g
    // -- should be normal
    constexpr Pantag am244g(95, 244);
    CHECK((from_NDI_SZA(95244) == am244g));
    CHECK(to_NDI_SZA(am244g) == 95244);

    // Am-244m1
    // -- Am-244m1 can take different values
    //    -- 1095244 (most cases)
    //    -- 95044 (specific cases)
    //       -- endf7act / 660nm
    constexpr Pantag am244m1(95, 244, Pantag::Index::metastable, 1);
    constexpr int default_am244m1 = 1095244;
    constexpr int alternate_am244m1 = 95044;
    CHECK((from_NDI_SZA(default_am244m1) == am244m1));
    CHECK((from_NDI_SZA(alternate_am244m1) == am244m1));
    CHECK(to_NDI_SZA(am244m1) == default_am244m1);
    CHECK(to_NDI_SZA(am244m1, "lanl2006") == default_am244m1);
    CHECK(to_NDI_SZA(am244m1, "060nm") == default_am244m1);
    CHECK(to_NDI_SZA(am244m1, "060") == default_am244m1);
    CHECK(to_NDI_SZA(am244m1, 60) == default_am244m1);
    CHECK(to_NDI_SZA(am244m1, 0.060) == default_am244m1);
    CHECK(to_NDI_SZA(am244m1, "endf7act") == alternate_am244m1);
    CHECK(to_NDI_SZA(am244m1, "700nm") == alternate_am244m1);
    CHECK(to_NDI_SZA(am244m1, "700") == alternate_am244m1);
    CHECK(to_NDI_SZA(am244m1, 700) == alternate_am244m1);
    CHECK(to_NDI_SZA(am244m1, 0.700) == alternate_am244m1);

    // Am-244m2
    // -- should be normal
    constexpr Pantag am244m2(95, 244, Pantag::Index::metastable, 2);
    CHECK((from_NDI_SZA(2095244) == am244m2));
    CHECK(to_NDI_SZA(am244m2) == 2095244);
}

// ================================================================================================

TEST_CASE("format: NDI FPID", "[tag][format][NDI]")
{
    using nautilus::tag::from_NDI_FPID;
    using nautilus::tag::Pantag;
    using nautilus::tag::to_NDI_FPID;

    // Particles
    constexpr Pantag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_FPID(1.999) == neutron));
    CHECK(to_NDI_FPID(neutron, 0.999) == 1.999);
    CHECK(to_NDI_FPID(neutron, 999) == 1.999);
    CHECK(to_NDI_FPID(neutron, "999") == 1.999);
    CHECK(to_NDI_FPID(neutron, "999nm") == 1.999);

    constexpr Pantag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_FPID(0.000) == photon));
    CHECK(to_NDI_FPID(photon, 0.456) == 0.456);
    CHECK(to_NDI_FPID(photon, 789) == 0.789);
    CHECK(to_NDI_FPID(photon, "123") == 0.123);
    CHECK(to_NDI_FPID(photon, "369nm") == 0.369);

    // Normal nuclides
    constexpr Pantag co59g(27, 59);
    CHECK((from_NDI_FPID(27059.123) == co59g));
    CHECK(to_NDI_FPID(co59g, 0.234) == 27059.234);
    CHECK(to_NDI_FPID(co59g, 345) == 27059.345);
    CHECK(to_NDI_FPID(co59g, "456") == 27059.456);
    CHECK(to_NDI_FPID(co59g, "567nm") == 27059.567);

    constexpr Pantag ta180m1(73, 180, Pantag::Index::metastable, 1);
    CHECK((from_NDI_FPID(1073180.987) == ta180m1));
    CHECK(to_NDI_FPID(ta180m1, 0.876) == 1073180.876);
    CHECK(to_NDI_FPID(ta180m1, 765) == 1073180.765);
    CHECK(to_NDI_FPID(ta180m1, "654") == 1073180.654);
    CHECK(to_NDI_FPID(ta180m1, "543nm") == 1073180.543);

    constexpr Pantag k38m2(19, 38, Pantag::Index::metastable, 2);
    CHECK((from_NDI_FPID(2019038.123) == k38m2));
    CHECK(to_NDI_FPID(k38m2, 0.135) == 2019038.135);
    CHECK(to_NDI_FPID(k38m2, 147) == 2019038.147);
    CHECK(to_NDI_FPID(k38m2, "159") == 2019038.159);
    CHECK(to_NDI_FPID(k38m2, "111nm") == 2019038.111);

    // Special cases

    // Am-242g
    // -- Am-242g and Am-242m1 are swapped in NDI FPID
    // -- Am-242g can take different values
    //    -- 1095242 (most cases)
    //    -- 95042 (specific cases)
    //       -- mendf70x / 701nm
    //       -- mtmg01 / 121nm - 135nm
    //       -- mtmg01ex / 121nm - 135nm
    constexpr Pantag am242g(95, 242);
    CHECK((from_NDI_FPID(1095242.120) == am242g)); // suffix that should be 1095242
    CHECK((from_NDI_FPID(1095242.123) == am242g)); // suffix that should be 95042 if writing
    CHECK((from_NDI_FPID(95042.120) == am242g)); // suffix that should be 1095242 if writing
    CHECK((from_NDI_FPID(95042.123) == am242g)); // suffix that should be 95042
    CHECK(to_NDI_FPID(am242g, "083") == 1095242.083);
    CHECK(to_NDI_FPID(am242g, "083nm") == 1095242.083);
    CHECK(to_NDI_FPID(am242g, 83) == 1095242.083);
    CHECK(to_NDI_FPID(am242g, 0.083) == 1095242.083);
    CHECK(to_NDI_FPID(am242g, "701") == 95042.701);
    CHECK(to_NDI_FPID(am242g, "701nm") == 95042.701);
    CHECK(to_NDI_FPID(am242g, 701) == 95042.701);
    CHECK(to_NDI_FPID(am242g, 0.701) == 95042.701);
    CHECK(to_NDI_FPID(am242g, "121") == 95042.121);
    CHECK(to_NDI_FPID(am242g, "128") == 95042.128);
    CHECK(to_NDI_FPID(am242g, "135") == 95042.135);
    CHECK(to_NDI_FPID(am242g, "121nm") == 95042.121);
    CHECK(to_NDI_FPID(am242g, "128nm") == 95042.128);
    CHECK(to_NDI_FPID(am242g, "135nm") == 95042.135);
    CHECK(to_NDI_FPID(am242g, 121) == 95042.121);
    CHECK(to_NDI_FPID(am242g, 128) == 95042.128);
    CHECK(to_NDI_FPID(am242g, 135) == 95042.135);
    CHECK(to_NDI_FPID(am242g, 0.121) == 95042.121);
    CHECK(to_NDI_FPID(am242g, 0.128) == 95042.128);
    CHECK(to_NDI_FPID(am242g, 0.135) == 95042.135);

    /*

    // Am-242m1
    // -- Am-242g and Am-242m1 are swapped in NDI FPID
    constexpr Pantag am242m1(95, 242, Pantag::Index::metastable, 1);
    CHECK((from_NDI_FPID(95242) == am242m1));
    CHECK(to_NDI_FPID(am242m1) == 95242);

    // Am-242m2
    // -- should be normal
    constexpr Pantag am242m2(95, 242, Pantag::Index::metastable, 2);
    CHECK((from_NDI_FPID(2095242) == am242m2));
    CHECK(to_NDI_FPID(am242m2) == 2095242);

    // Am-243g
    // -- should be normal
    constexpr Pantag am243g(95, 243);
    CHECK((from_NDI_FPID(95243) == am243g));
    CHECK(to_NDI_FPID(am243g) == 95243);

    // Am-243m1
    // -- should be normal
    constexpr Pantag am243m1(95, 243, Pantag::Index::metastable, 1);
    CHECK((from_NDI_FPID(1095243) == am243m1));
    CHECK(to_NDI_FPID(am243m1) == 1095243);

    // Am-243m2
    // -- should be normal
    constexpr Pantag am243m2(95, 243, Pantag::Index::metastable, 2);
    CHECK((from_NDI_FPID(2095243) == am243m2));
    CHECK(to_NDI_FPID(am243m2) == 2095243);

    // Am-244g
    // -- should be normal
    constexpr Pantag am244g(95, 244);
    CHECK((from_NDI_FPID(95244) == am244g));
    CHECK(to_NDI_FPID(am244g) == 95244);

    // Am-244m1
    // -- Am-244m1 can take different values
    //    -- 1095244 (most cases)
    //    -- 95044 (specific cases)
    //       -- endf7act / 660nm
    constexpr Pantag am244m1(95, 244, Pantag::Index::metastable, 1);
    constexpr int default_am244m1 = 1095244;
    constexpr int alternate_am244m1 = 95044;
    CHECK((from_NDI_FPID(default_am244m1) == am244m1));
    CHECK((from_NDI_FPID(alternate_am244m1) == am244m1));
    CHECK(to_NDI_FPID(am244m1) == default_am244m1);
    CHECK(to_NDI_FPID(am244m1, "lanl2006") == default_am244m1);
    CHECK(to_NDI_FPID(am244m1, "060nm") == default_am244m1);
    CHECK(to_NDI_FPID(am244m1, 60) == default_am244m1);
    CHECK(to_NDI_FPID(am244m1, 0.060) == default_am244m1);
    CHECK(to_NDI_FPID(am244m1, "endf7act") == alternate_am244m1);
    CHECK(to_NDI_FPID(am244m1, "700nm") == alternate_am244m1);
    CHECK(to_NDI_FPID(am244m1, 700) == alternate_am244m1);
    CHECK(to_NDI_FPID(am244m1, 0.700) == alternate_am244m1);

    // Am-244m2
    // -- should be normal
    constexpr Pantag am244m2(95, 244, Pantag::Index::metastable, 2);
    CHECK((from_NDI_FPID(2095244) == am244m2));
    CHECK(to_NDI_FPID(am244m2) == 2095244);
*/}

// ================================================================================================

// TODO: NDI zaid
// TODO: NDI short string
