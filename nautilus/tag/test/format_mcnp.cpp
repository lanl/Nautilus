#include <array>
#include <cstdio>
#include <fstream>
#include <string>

#include "nautilus/tag/format_mcnp.hpp"
#include "nautilus/tag/names.hpp"
#include "nautilus/tag/pantag.hpp"

#include <catch2/catch_test_macros.hpp>

// Note that the Catch2 magic doesn't always play well with Nautilus.  See
// https://stackoverflow.com/q/59770581/1791919 for a discussion of a related problem.

// ================================================================================================

namespace detail {

auto load_isotopes() {
    // Open the file
    std::ifstream fin("isotopes.dat");
    CHECK(fin.is_open());
    // Data storage
    struct element {
        int Z;
        std::vector<int> As;
        element(int atomic_number)
            : Z{atomic_number}
        {}
    };
    std::vector<element> elements;
    // Loop over the file
    std::string line;
    const std::string ws = " \t";
    while (std::getline(fin, line)) {
        // trim leading and trailing whitespace
        line = line.substr(line.find_first_not_of(ws));
        line = line.substr(0, line.find_last_not_of(ws) + 1);
        // get the atomic number
        auto pos = line.find_first_of(':');
        const int Z = std::stoi(line.substr(0, pos));
        elements.emplace_back(Z);
        // loop over the atomic mass numbers
        ++pos; // skip the colon
        while (pos != std::string::npos) {
            auto pos0 = line.find_first_not_of(ws, pos);
            pos = line.find_first_of(ws, pos0);
            const int A = std::stoi(line.substr(pos0, pos));
            elements.back().As.push_back(A);
        }
    }
    return elements;
}

}; // end namespace detail

// ================================================================================================

TEST_CASE("format: MCNP partial zaid", "[tag][format][MCNP]")
{
    using nautilus::tag::from_MCNP_partial_zaid;
    using nautilus::tag::Pantag;
    using nautilus::tag::to_MCNP_partial_zaid;

    // TODO: Should I get the "unknown partial zaid" value from format_mcnp.hpp?

    // Normal nuclides
    const auto elements = detail::load_isotopes();
    for (const auto & element : elements) {
        const int Z = element.Z;
        for (const int A : element.As) {
            if ((Z == 95) && (A == 242)) {
                // skip Am-242, because it's a special case tested below
                break;
            }
            // The MCNP manual notes that excited states can be m = 1, 2, 3, or 4
            for (int m = 0; m < 5; ++m) {
                Pantag tag(Z, A, Pantag::Index::metastable, m);
                int partial_zaid = Z * 1000 + A;
                if (m != 0) {
                    partial_zaid += 300 + m * 100;
                }
                INFO("checking Z = " << Z << ", A = " << A << ", m = " << m << " (partial zaid = "
                        << partial_zaid << ")");
                CHECK((from_MCNP_partial_zaid(partial_zaid) == tag));
                CHECK(to_MCNP_partial_zaid(tag) == partial_zaid);
            }
        }
    }

    // Special cases

    // Am-242g and Am-242m1 are swapped
    // m == 0
    constexpr Pantag am242g_tag(95, 242);
    constexpr int am242g_zaid = 95642; // should be 95242 except it's swapped with Am-242m1
    CHECK((from_MCNP_partial_zaid(am242g_zaid) == am242g_tag));
    CHECK(to_MCNP_partial_zaid(am242g_tag) == am242g_zaid);
    // m = 1
    constexpr Pantag am242m1_tag(95, 242, Pantag::Index::metastable, 1);
    constexpr int am242m1_zaid = 95242; // should be 95642 except it's swapped with Am-242g
    CHECK((from_MCNP_partial_zaid(am242m1_zaid) == am242m1_tag));
    CHECK(to_MCNP_partial_zaid(am242m1_tag) == am242m1_zaid);
    // m = 2
    constexpr Pantag am242m2_tag(95, 242, Pantag::Index::metastable, 2);
    constexpr int am242m2_zaid = 95742; // follows the standard notation
    CHECK((from_MCNP_partial_zaid(am242m2_zaid) == am242m2_tag));
    CHECK(to_MCNP_partial_zaid(am242m2_tag) == am242m2_zaid);
    // m = 3
    constexpr Pantag am242m3_tag(95, 242, Pantag::Index::metastable, 3);
    constexpr int am242m3_zaid = 95842; // follows the standard notation
    CHECK((from_MCNP_partial_zaid(am242m3_zaid) == am242m3_tag));
    CHECK(to_MCNP_partial_zaid(am242m3_tag) == am242m3_zaid);
    // m = 4
    constexpr Pantag am242m4_tag(95, 242, Pantag::Index::metastable, 4);
    constexpr int am242m4_zaid = 95942; // follows the standard notation
    CHECK((from_MCNP_partial_zaid(am242m4_zaid) == am242m4_tag));
    CHECK(to_MCNP_partial_zaid(am242m4_tag) == am242m4_zaid);

    // m > 4 is not allowed
    // -- 200 <= A < 300 with m = 5 would lead to overflow that changes the Z value, so in general
    //    m > 4 is not permitted
    CHECK(to_MCNP_partial_zaid(6812) == -1);
    CHECK(to_MCNP_partial_zaid(6912) == -1);
    CHECK(to_MCNP_partial_zaid(47907) == -1);

    // elementals
    CHECK((from_MCNP_partial_zaid(6000) == Pantag(6, Pantag::elemental)));
    CHECK(to_MCNP_partial_zaid(Pantag(13, Pantag::elemental)) == 13000);

    // particles
    CHECK(to_MCNP_partial_zaid(Pantag(nautilus::tag::names::proton)) == -1);
    CHECK(to_MCNP_partial_zaid(Pantag(nautilus::tag::names::muon)) == -1);

    // user
    CHECK(to_MCNP_partial_zaid(Pantag(Pantag::user, 0)) == -1);

    // unknown
    CHECK(to_MCNP_partial_zaid(Pantag(Pantag::unknown)) == -1);
}

// ================================================================================================

// TODO: MCNP full zaid
// TODO: MCNP particle symbol

/*TEST_CASE("format: NDI zaid", "[tag][format][NDI]")
{
    using nautilus::tag::from_NDI_zaid;
    using nautilus::tag::Pantag;
    using nautilus::tag::to_NDI_zaid;

    // Particles
    constexpr Pantag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_zaid("1.999nm") == neutron));
    CHECK(to_NDI_zaid(neutron, 0.999) == "1.999nm");
    CHECK(to_NDI_zaid(neutron, 999) == "1.999nm");
    CHECK(to_NDI_zaid(neutron, "999") == "1.999nm");
    CHECK(to_NDI_zaid(neutron, "999nm") == "1.999nm");

    constexpr Pantag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_zaid("0.000nm") == photon));
    CHECK(to_NDI_zaid(photon, 0.456) == "0.456nm");
    CHECK(to_NDI_zaid(photon, 789) == "0.789nm");
    CHECK(to_NDI_zaid(photon, "123") == "0.123nm");
    CHECK(to_NDI_zaid(photon, "369nm") == "0.369nm");

    // Normal nuclides
    constexpr Pantag co59g(27, 59);
    CHECK((from_NDI_zaid("27059.123nm") == co59g));
    CHECK(to_NDI_zaid(co59g, 0.234) == "27059.234nm");
    CHECK(to_NDI_zaid(co59g, 345) == "27059.345nm");
    CHECK(to_NDI_zaid(co59g, "456") == "27059.456nm");
    CHECK(to_NDI_zaid(co59g, "567nm") == "27059.567nm");

    constexpr Pantag ta180m1(73, 180, Pantag::Index::metastable, 1);
    CHECK((from_NDI_zaid("1073180.987nm") == ta180m1));
    CHECK(to_NDI_zaid(ta180m1, 0.876) == "1073180.876nm");
    CHECK(to_NDI_zaid(ta180m1, 765) == "1073180.765nm");
    CHECK(to_NDI_zaid(ta180m1, "654") == "1073180.654nm");
    CHECK(to_NDI_zaid(ta180m1, "543nm") == "1073180.543nm");

    constexpr Pantag k38m2(19, 38, Pantag::Index::metastable, 2);
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
    constexpr Pantag am242g(95, 242);
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
    constexpr Pantag am242m1(95, 242, Pantag::Index::metastable, 1);
    CHECK((from_NDI_zaid("95242.133nm") == am242m1));
    CHECK(to_NDI_zaid(am242m1, 0.134) == "95242.134nm");
    CHECK(to_NDI_zaid(am242m1, 135) == "95242.135nm");
    CHECK(to_NDI_zaid(am242m1, "136") == "95242.136nm");
    CHECK(to_NDI_zaid(am242m1, "137nm") == "95242.137nm");

    // Am-242m2
    // -- should be normal
    constexpr Pantag am242m2(95, 242, Pantag::Index::metastable, 2);
    CHECK((from_NDI_zaid("2095242.111nm") == am242m2));
    CHECK(to_NDI_zaid(am242m2, 0.333) == "2095242.333nm");
    CHECK(to_NDI_zaid(am242m2, 555) == "2095242.555nm");
    CHECK(to_NDI_zaid(am242m2, "777") == "2095242.777nm");
    CHECK(to_NDI_zaid(am242m2, "999nm") == "2095242.999nm");

    // Am-243g
    // -- should be normal
    constexpr Pantag am243g(95, 243);
    CHECK((from_NDI_zaid("95243.867nm") == am243g));
    CHECK(to_NDI_zaid(am243g, 0.675) == "95243.675nm");
    CHECK(to_NDI_zaid(am243g, 753) == "95243.753nm");
    CHECK(to_NDI_zaid(am243g, "530") == "95243.530nm");
    CHECK(to_NDI_zaid(am243g, "309nm") == "95243.309nm");

    // Am-243m1
    // -- should be normal
    constexpr Pantag am243m1(95, 243, Pantag::Index::metastable, 1);
    CHECK((from_NDI_zaid("1095243.555nm") == am243m1));
    CHECK(to_NDI_zaid(am243m1, 0.555) == "1095243.555nm");
    CHECK(to_NDI_zaid(am243m1, 555) == "1095243.555nm");
    CHECK(to_NDI_zaid(am243m1, "555") == "1095243.555nm");
    CHECK(to_NDI_zaid(am243m1, "555nm") == "1095243.555nm");

    // Am-243m2
    // -- should be normal
    constexpr Pantag am243m2(95, 243, Pantag::Index::metastable, 2);
    CHECK((from_NDI_zaid("2095243.135nm") == am243m2));
    CHECK(to_NDI_zaid(am243m2, 0.135) == "2095243.135nm");
    CHECK(to_NDI_zaid(am243m2, 135) == "2095243.135nm");
    CHECK(to_NDI_zaid(am243m2, "135") == "2095243.135nm");
    CHECK(to_NDI_zaid(am243m2, "135nm") == "2095243.135nm");

    // Am-244g
    // -- should be normal
    constexpr Pantag am244g(95, 244);
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
    constexpr Pantag am244m1(95, 244, Pantag::Index::metastable, 1);
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
    constexpr Pantag am244m2(95, 244, Pantag::Index::metastable, 2);
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
    using nautilus::tag::Pantag;
    using nautilus::tag::to_NDI_short_string;

    // Particles
    constexpr Pantag neutron(nautilus::tag::names::neutron);
    CHECK((from_NDI_short_string("n") == neutron));
    CHECK(to_NDI_short_string(neutron) == "n");

    constexpr Pantag photon(nautilus::tag::names::photon);
    CHECK((from_NDI_short_string("g") == photon));
    CHECK((from_NDI_short_string("g0") == photon));
    CHECK(to_NDI_short_string(photon) == "g");

    // Normal nuclides
    constexpr Pantag co59g(27, 59);
    CHECK((from_NDI_short_string("co59") == co59g));
    CHECK(to_NDI_short_string(co59g) == "co59");

    constexpr Pantag ta180(73, 180);
    CHECK((from_NDI_short_string("ta180") == ta180));
    CHECK(to_NDI_short_string(ta180) == "ta180");

    constexpr Pantag k38(19, 38);
    CHECK((from_NDI_short_string("k38") == k38));
    CHECK(to_NDI_short_string(k38) == "k38");

    // Special cases

    // Am-242m1
    // -- Am-242g and Am-242m1 are swapped: "am242" -> Am-242m1; Am-242g not representable
    constexpr Pantag am242m1(95, 242, Pantag::Index::metastable, 1);
    CHECK((from_NDI_short_string("am242") == am242m1));
    CHECK(to_NDI_short_string(am242m1) == "am242");

    // Am-243g
    // -- should be normal
    constexpr Pantag am243g(95, 243);
    CHECK((from_NDI_short_string("am243") == am243g));
    CHECK(to_NDI_short_string(am243g) == "am243");

    // Am-244g
    // -- should be normal
    constexpr Pantag am244g(95, 244);
    CHECK((from_NDI_short_string("am244") == am244g));
    CHECK(to_NDI_short_string(am244g) == "am244");
}*/
