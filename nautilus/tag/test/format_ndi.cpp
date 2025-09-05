#include "nautilus/tag/format_ndi.hpp"
#include "nautilus/tag/names.hpp"
#include "nautilus/tag/entity_tag.hpp"

#include <catch2/catch_test_macros.hpp>

// Note that the Catch2 magic doesn't always play well with Nautilus.  See
// https://stackoverflow.com/q/59770581/1791919 for a discussion of a related problem.

// ================================================================================================

TEST_CASE("format: NDI SZA", "[tag][format][NDI]")
{
    using nautilus::tag::from_NDI_SZA;
    using nautilus::tag::EntityTag;
    using nautilus::tag::to_NDI_SZA;

    // Particles
    constexpr EntityTag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_SZA(1) == neutron));
    CHECK(to_NDI_SZA(neutron) == 1);

    constexpr EntityTag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_SZA(0) == photon));
    CHECK(to_NDI_SZA(photon) == 0);

    // Normal nuclides
    constexpr EntityTag co59g(27, 59);
    CHECK((from_NDI_SZA(27059) == co59g));
    CHECK(to_NDI_SZA(co59g) == 27059);

    constexpr EntityTag ta180m1(73, 180, 1);
    CHECK((from_NDI_SZA(1073180) == ta180m1));
    CHECK(to_NDI_SZA(ta180m1) == 1073180);

    constexpr EntityTag k38m2(19, 38, 2);
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
    constexpr EntityTag am242g(95, 242);
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
    constexpr EntityTag am242m1(95, 242, 1);
    CHECK((from_NDI_SZA(95242) == am242m1));
    CHECK(to_NDI_SZA(am242m1) == 95242);

    // Am-242m2
    // -- should be normal
    constexpr EntityTag am242m2(95, 242, 2);
    CHECK((from_NDI_SZA(2095242) == am242m2));
    CHECK(to_NDI_SZA(am242m2) == 2095242);

    // Am-243g
    // -- should be normal
    constexpr EntityTag am243g(95, 243);
    CHECK((from_NDI_SZA(95243) == am243g));
    CHECK(to_NDI_SZA(am243g) == 95243);

    // Am-243m1
    // -- should be normal
    constexpr EntityTag am243m1(95, 243, 1);
    CHECK((from_NDI_SZA(1095243) == am243m1));
    CHECK(to_NDI_SZA(am243m1) == 1095243);

    // Am-243m2
    // -- should be normal
    constexpr EntityTag am243m2(95, 243, 2);
    CHECK((from_NDI_SZA(2095243) == am243m2));
    CHECK(to_NDI_SZA(am243m2) == 2095243);

    // Am-244g
    // -- should be normal
    constexpr EntityTag am244g(95, 244);
    CHECK((from_NDI_SZA(95244) == am244g));
    CHECK(to_NDI_SZA(am244g) == 95244);

    // Am-244m1
    // -- Am-244m1 can take different values
    //    -- 1095244 (most cases)
    //    -- 95044 (specific cases)
    //       -- endf7act / 660nm
    constexpr EntityTag am244m1(95, 244, 1);
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
    constexpr EntityTag am244m2(95, 244, 2);
    CHECK((from_NDI_SZA(2095244) == am244m2));
    CHECK(to_NDI_SZA(am244m2) == 2095244);
}

// ================================================================================================

TEST_CASE("format: NDI FPID", "[tag][format][NDI]")
{
    using nautilus::tag::from_NDI_FPID;
    using nautilus::tag::EntityTag;
    using nautilus::tag::to_NDI_FPID;

    // Particles
    constexpr EntityTag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_FPID(1.999) == neutron));
    CHECK(to_NDI_FPID(neutron, 0.999) == 1.999);
    CHECK(to_NDI_FPID(neutron, 999) == 1.999);
    CHECK(to_NDI_FPID(neutron, "999") == 1.999);
    CHECK(to_NDI_FPID(neutron, "999nm") == 1.999);

    constexpr EntityTag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_FPID(0.000) == photon));
    CHECK(to_NDI_FPID(photon, 0.456) == 0.456);
    CHECK(to_NDI_FPID(photon, 789) == 0.789);
    CHECK(to_NDI_FPID(photon, "123") == 0.123);
    CHECK(to_NDI_FPID(photon, "369nm") == 0.369);

    // Normal nuclides
    constexpr EntityTag co59g(27, 59);
    CHECK((from_NDI_FPID(27059.123) == co59g));
    CHECK(to_NDI_FPID(co59g, 0.234) == 27059.234);
    CHECK(to_NDI_FPID(co59g, 345) == 27059.345);
    CHECK(to_NDI_FPID(co59g, "456") == 27059.456);
    CHECK(to_NDI_FPID(co59g, "567nm") == 27059.567);

    constexpr EntityTag ta180m1(73, 180, 1);
    CHECK((from_NDI_FPID(1073180.987) == ta180m1));
    CHECK(to_NDI_FPID(ta180m1, 0.876) == 1073180.876);
    CHECK(to_NDI_FPID(ta180m1, 765) == 1073180.765);
    CHECK(to_NDI_FPID(ta180m1, "654") == 1073180.654);
    CHECK(to_NDI_FPID(ta180m1, "543nm") == 1073180.543);

    constexpr EntityTag k38m2(19, 38, 2);
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
    constexpr EntityTag am242g(95, 242);
    CHECK((from_NDI_FPID(1095242.120) == am242g)); // suffix that should be 1095242
    CHECK((from_NDI_FPID(1095242.123) == am242g)); // suffix that should be 95042 if writing
    CHECK((from_NDI_FPID(95042.120) == am242g));   // suffix that should be 1095242 if writing
    CHECK((from_NDI_FPID(95042.123) == am242g));   // suffix that should be 95042
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

    // Am-242m1
    // -- Am-242g and Am-242m1 are swapped in NDI FPID
    constexpr EntityTag am242m1(95, 242, 1);
    CHECK((from_NDI_FPID(95242.133) == am242m1));
    CHECK(to_NDI_FPID(am242m1, 0.134) == 95242.134);
    CHECK(to_NDI_FPID(am242m1, 135) == 95242.135);
    CHECK(to_NDI_FPID(am242m1, "136") == 95242.136);
    CHECK(to_NDI_FPID(am242m1, "137nm") == 95242.137);

    // Am-242m2
    // -- should be normal
    constexpr EntityTag am242m2(95, 242, 2);
    CHECK((from_NDI_FPID(2095242.111) == am242m2));
    CHECK(to_NDI_FPID(am242m2, 0.333) == 2095242.333);
    CHECK(to_NDI_FPID(am242m2, 555) == 2095242.555);
    CHECK(to_NDI_FPID(am242m2, "777") == 2095242.777);
    CHECK(to_NDI_FPID(am242m2, "999nm") == 2095242.999);

    // Am-243g
    // -- should be normal
    constexpr EntityTag am243g(95, 243);
    CHECK((from_NDI_FPID(95243.867) == am243g));
    CHECK(to_NDI_FPID(am243g, 0.675) == 95243.675);
    CHECK(to_NDI_FPID(am243g, 753) == 95243.753);
    CHECK(to_NDI_FPID(am243g, "530") == 95243.530);
    CHECK(to_NDI_FPID(am243g, "309nm") == 95243.309);

    // Am-243m1
    // -- should be normal
    constexpr EntityTag am243m1(95, 243, 1);
    CHECK((from_NDI_FPID(1095243.555) == am243m1));
    CHECK(to_NDI_FPID(am243m1, 0.555) == 1095243.555);
    CHECK(to_NDI_FPID(am243m1, 555) == 1095243.555);
    CHECK(to_NDI_FPID(am243m1, "555") == 1095243.555);
    CHECK(to_NDI_FPID(am243m1, "555nm") == 1095243.555);

    // Am-243m2
    // -- should be normal
    constexpr EntityTag am243m2(95, 243, 2);
    CHECK((from_NDI_FPID(2095243.135) == am243m2));
    CHECK(to_NDI_FPID(am243m2, 0.135) == 2095243.135);
    CHECK(to_NDI_FPID(am243m2, 135) == 2095243.135);
    CHECK(to_NDI_FPID(am243m2, "135") == 2095243.135);
    CHECK(to_NDI_FPID(am243m2, "135nm") == 2095243.135);

    // Am-244g
    // -- should be normal
    constexpr EntityTag am244g(95, 244);
    CHECK((from_NDI_FPID(95244.951) == am244g));
    CHECK(to_NDI_FPID(am244g, 0.951) == 95244.951);
    CHECK(to_NDI_FPID(am244g, 951) == 95244.951);
    CHECK(to_NDI_FPID(am244g, "951") == 95244.951);
    CHECK(to_NDI_FPID(am244g, "951nm") == 95244.951);

    // Am-244m1
    // -- Am-244m1 can take different values
    //    -- 1095244 (most cases)
    //    -- 95044 (specific cases)
    //       -- endf7act / 660nm
    constexpr EntityTag am244m1(95, 244, 1);
    CHECK((from_NDI_FPID(1095244.600) == am244m1)); // suffix that should be 1095244
    CHECK((from_NDI_FPID(1095244.700) == am244m1)); // suffix that should be 95044 if writing
    CHECK((from_NDI_FPID(95044.600) == am244m1));   // suffix that should be 1095244 if writing
    CHECK((from_NDI_FPID(95044.700) == am244m1));   // suffix that should be 95044
    CHECK(to_NDI_FPID(am244m1, "060nm") == 1095244.060);
    CHECK(to_NDI_FPID(am244m1, "060") == 1095244.060);
    CHECK(to_NDI_FPID(am244m1, 60) == 1095244.060);
    CHECK(to_NDI_FPID(am244m1, 0.060) == 1095244.060);
    CHECK(to_NDI_FPID(am244m1, "700nm") == 95044.700);
    CHECK(to_NDI_FPID(am244m1, "700") == 95044.700);
    CHECK(to_NDI_FPID(am244m1, 700) == 95044.700);
    CHECK(to_NDI_FPID(am244m1, 0.700) == 95044.700);

    // Am-244m2
    // -- should be normal
    constexpr EntityTag am244m2(95, 244, 2);
    CHECK((from_NDI_FPID(2095244.700) == am244m2));
    CHECK(to_NDI_FPID(am244m2, 0.701) == 2095244.701);
    CHECK(to_NDI_FPID(am244m2, 702) == 2095244.702);
    CHECK(to_NDI_FPID(am244m2, "703") == 2095244.703);
    CHECK(to_NDI_FPID(am244m2, "704nm") == 2095244.704);
}

// ================================================================================================

TEST_CASE("format: NDI zaid", "[tag][format][NDI]")
{
    using nautilus::tag::from_NDI_zaid;
    using nautilus::tag::EntityTag;
    using nautilus::tag::to_NDI_zaid;

    // Particles
    constexpr EntityTag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_zaid("1.999nm") == neutron));
    CHECK(to_NDI_zaid(neutron, 0.999) == "1.999nm");
    CHECK(to_NDI_zaid(neutron, 999) == "1.999nm");
    CHECK(to_NDI_zaid(neutron, "999") == "1.999nm");
    CHECK(to_NDI_zaid(neutron, "999nm") == "1.999nm");

    constexpr EntityTag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_zaid("0.000nm") == photon));
    CHECK(to_NDI_zaid(photon, 0.456) == "0.456nm");
    CHECK(to_NDI_zaid(photon, 789) == "0.789nm");
    CHECK(to_NDI_zaid(photon, "123") == "0.123nm");
    CHECK(to_NDI_zaid(photon, "369nm") == "0.369nm");

    // Normal nuclides
    constexpr EntityTag co59g(27, 59);
    CHECK((from_NDI_zaid("27059.123nm") == co59g));
    CHECK(to_NDI_zaid(co59g, 0.234) == "27059.234nm");
    CHECK(to_NDI_zaid(co59g, 345) == "27059.345nm");
    CHECK(to_NDI_zaid(co59g, "456") == "27059.456nm");
    CHECK(to_NDI_zaid(co59g, "567nm") == "27059.567nm");

    constexpr EntityTag ta180m1(73, 180, 1);
    CHECK((from_NDI_zaid("1073180.987nm") == ta180m1));
    CHECK(to_NDI_zaid(ta180m1, 0.876) == "1073180.876nm");
    CHECK(to_NDI_zaid(ta180m1, 765) == "1073180.765nm");
    CHECK(to_NDI_zaid(ta180m1, "654") == "1073180.654nm");
    CHECK(to_NDI_zaid(ta180m1, "543nm") == "1073180.543nm");

    constexpr EntityTag k38m2(19, 38, 2);
    CHECK((from_NDI_zaid("2019038.123nm") == k38m2));
    CHECK(to_NDI_zaid(k38m2, 0.000) == "2019038.000nm");
    CHECK(to_NDI_zaid(k38m2, 5) == "2019038.005nm");
    CHECK(to_NDI_zaid(k38m2, "050") == "2019038.050nm");
    CHECK(to_NDI_zaid(k38m2, "500nm") == "2019038.500nm");

    // Special cases

    // Am-242g
    // -- Am-242g and Am-242m1 are swapped in NDI zaid
    // -- Am-242g can take different values
    //    -- 1095242 (most cases)
    //    -- 95042 (specific cases)
    //       -- mendf70x / 701nm
    //       -- mtmg01 / 121nm - 135nm
    //       -- mtmg01ex / 121nm - 135nm
    constexpr EntityTag am242g(95, 242);
    CHECK((from_NDI_zaid("1095242.120nm") == am242g)); // suffix that should be 1095242
    CHECK((from_NDI_zaid("1095242.123nm") == am242g)); // suffix that should be 95042 if writing
    CHECK((from_NDI_zaid("95042.120nm") == am242g));   // suffix that should be 1095242 if writing
    CHECK((from_NDI_zaid("95042.123nm") == am242g));   // suffix that should be 95042
    CHECK(to_NDI_zaid(am242g, "083") == "1095242.083nm");
    CHECK(to_NDI_zaid(am242g, "083nm") == "1095242.083nm");
    CHECK(to_NDI_zaid(am242g, 83) == "1095242.083nm");
    CHECK(to_NDI_zaid(am242g, 0.083) == "1095242.083nm");
    CHECK(to_NDI_zaid(am242g, "701") == "95042.701nm");
    CHECK(to_NDI_zaid(am242g, "701nm") == "95042.701nm");
    CHECK(to_NDI_zaid(am242g, 701) == "95042.701nm");
    CHECK(to_NDI_zaid(am242g, 0.701) == "95042.701nm");
    CHECK(to_NDI_zaid(am242g, "121") == "95042.121nm");
    CHECK(to_NDI_zaid(am242g, "128") == "95042.128nm");
    CHECK(to_NDI_zaid(am242g, "135") == "95042.135nm");
    CHECK(to_NDI_zaid(am242g, "121nm") == "95042.121nm");
    CHECK(to_NDI_zaid(am242g, "128nm") == "95042.128nm");
    CHECK(to_NDI_zaid(am242g, "135nm") == "95042.135nm");
    CHECK(to_NDI_zaid(am242g, 121) == "95042.121nm");
    CHECK(to_NDI_zaid(am242g, 128) == "95042.128nm");
    CHECK(to_NDI_zaid(am242g, 135) == "95042.135nm");
    CHECK(to_NDI_zaid(am242g, 0.121) == "95042.121nm");
    CHECK(to_NDI_zaid(am242g, 0.128) == "95042.128nm");
    CHECK(to_NDI_zaid(am242g, 0.135) == "95042.135nm");

    // Am-242m1
    // -- Am-242g and Am-242m1 are swapped in NDI zaid
    constexpr EntityTag am242m1(95, 242, 1);
    CHECK((from_NDI_zaid("95242.133nm") == am242m1));
    CHECK(to_NDI_zaid(am242m1, 0.134) == "95242.134nm");
    CHECK(to_NDI_zaid(am242m1, 135) == "95242.135nm");
    CHECK(to_NDI_zaid(am242m1, "136") == "95242.136nm");
    CHECK(to_NDI_zaid(am242m1, "137nm") == "95242.137nm");

    // Am-242m2
    // -- should be normal
    constexpr EntityTag am242m2(95, 242, 2);
    CHECK((from_NDI_zaid("2095242.111nm") == am242m2));
    CHECK(to_NDI_zaid(am242m2, 0.333) == "2095242.333nm");
    CHECK(to_NDI_zaid(am242m2, 555) == "2095242.555nm");
    CHECK(to_NDI_zaid(am242m2, "777") == "2095242.777nm");
    CHECK(to_NDI_zaid(am242m2, "999nm") == "2095242.999nm");

    // Am-243g
    // -- should be normal
    constexpr EntityTag am243g(95, 243);
    CHECK((from_NDI_zaid("95243.867nm") == am243g));
    CHECK(to_NDI_zaid(am243g, 0.675) == "95243.675nm");
    CHECK(to_NDI_zaid(am243g, 753) == "95243.753nm");
    CHECK(to_NDI_zaid(am243g, "530") == "95243.530nm");
    CHECK(to_NDI_zaid(am243g, "309nm") == "95243.309nm");

    // Am-243m1
    // -- should be normal
    constexpr EntityTag am243m1(95, 243, 1);
    CHECK((from_NDI_zaid("1095243.555nm") == am243m1));
    CHECK(to_NDI_zaid(am243m1, 0.555) == "1095243.555nm");
    CHECK(to_NDI_zaid(am243m1, 555) == "1095243.555nm");
    CHECK(to_NDI_zaid(am243m1, "555") == "1095243.555nm");
    CHECK(to_NDI_zaid(am243m1, "555nm") == "1095243.555nm");

    // Am-243m2
    // -- should be normal
    constexpr EntityTag am243m2(95, 243, 2);
    CHECK((from_NDI_zaid("2095243.135nm") == am243m2));
    CHECK(to_NDI_zaid(am243m2, 0.135) == "2095243.135nm");
    CHECK(to_NDI_zaid(am243m2, 135) == "2095243.135nm");
    CHECK(to_NDI_zaid(am243m2, "135") == "2095243.135nm");
    CHECK(to_NDI_zaid(am243m2, "135nm") == "2095243.135nm");

    // Am-244g
    // -- should be normal
    constexpr EntityTag am244g(95, 244);
    CHECK((from_NDI_zaid("95244.951nm") == am244g));
    CHECK(to_NDI_zaid(am244g, 0.951) == "95244.951nm");
    CHECK(to_NDI_zaid(am244g, 951) == "95244.951nm");
    CHECK(to_NDI_zaid(am244g, "951") == "95244.951nm");
    CHECK(to_NDI_zaid(am244g, "951nm") == "95244.951nm");

    // Am-244m1
    // -- Am-244m1 can take different values
    //    -- 1095244 (most cases)
    //    -- 95044 (specific cases)
    //       -- endf7act / 660nm
    constexpr EntityTag am244m1(95, 244, 1);
    CHECK((from_NDI_zaid("1095244.600nm") == am244m1)); // suffix that should be 1095244
    CHECK((from_NDI_zaid("1095244.700nm") == am244m1)); // suffix that should be 95044 if writing
    CHECK((from_NDI_zaid("95044.600nm") == am244m1));   // suffix that should be 1095244 if writing
    CHECK((from_NDI_zaid("95044.700nm") == am244m1));   // suffix that should be 95044
    CHECK(to_NDI_zaid(am244m1, "060nm") == "1095244.060nm");
    CHECK(to_NDI_zaid(am244m1, "060") == "1095244.060nm");
    CHECK(to_NDI_zaid(am244m1, 60) == "1095244.060nm");
    CHECK(to_NDI_zaid(am244m1, 0.060) == "1095244.060nm");
    CHECK(to_NDI_zaid(am244m1, "700nm") == "95044.700nm");
    CHECK(to_NDI_zaid(am244m1, "700") == "95044.700nm");
    CHECK(to_NDI_zaid(am244m1, 700) == "95044.700nm");
    CHECK(to_NDI_zaid(am244m1, 0.700) == "95044.700nm");

    // Am-244m2
    // -- should be normal
    constexpr EntityTag am244m2(95, 244, 2);
    CHECK((from_NDI_zaid("2095244.700nm") == am244m2));
    CHECK(to_NDI_zaid(am244m2, 0.701) == "2095244.701nm");
    CHECK(to_NDI_zaid(am244m2, 702) == "2095244.702nm");
    CHECK(to_NDI_zaid(am244m2, "703") == "2095244.703nm");
    CHECK(to_NDI_zaid(am244m2, "704nm") == "2095244.704nm");
}

// ================================================================================================

TEST_CASE("format: NDI short string", "[tag][format][NDI]")
{
    using nautilus::tag::from_NDI_short_string;
    using nautilus::tag::EntityTag;
    using nautilus::tag::to_NDI_short_string;

    // Particles
    constexpr EntityTag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_short_string("n") == neutron));
    CHECK(to_NDI_short_string(neutron) == "n");

    constexpr EntityTag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_short_string("g") == photon));
    CHECK((from_NDI_short_string("g0") == photon));
    CHECK(to_NDI_short_string(photon) == "g");

    // Normal nuclides
    constexpr EntityTag co59g(27, 59);
    CHECK((from_NDI_short_string("co59") == co59g));
    CHECK(to_NDI_short_string(co59g) == "co59");

    constexpr EntityTag ta180(73, 180);
    CHECK((from_NDI_short_string("ta180") == ta180));
    CHECK(to_NDI_short_string(ta180) == "ta180");

    constexpr EntityTag k38(19, 38);
    CHECK((from_NDI_short_string("k38") == k38));
    CHECK(to_NDI_short_string(k38) == "k38");

    // Special cases

    // Am-242m1
    // -- Am-242g and Am-242m1 are swapped: "am242" -> Am-242m1; Am-242g not representable
    constexpr EntityTag am242m1(95, 242, 1);
    CHECK((from_NDI_short_string("am242") == am242m1));
    CHECK(to_NDI_short_string(am242m1) == "am242");

    // Am-243g
    // -- should be normal
    constexpr EntityTag am243g(95, 243);
    CHECK((from_NDI_short_string("am243") == am243g));
    CHECK(to_NDI_short_string(am243g) == "am243");

    // Am-244g
    // -- should be normal
    constexpr EntityTag am244g(95, 244);
    CHECK((from_NDI_short_string("am244") == am244g));
    CHECK(to_NDI_short_string(am244g) == "am244");
}
