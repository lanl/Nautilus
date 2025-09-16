#include <array>
#include <cstdio>
#include <fstream>
#include <string>

#include "nautilus/entity_tag/entity_tag.hpp"
#include "nautilus/entity_tag/format_mcnp.hpp"
#include "nautilus/entity_tag/names.hpp"

#include <catch2/catch_test_macros.hpp>

// Note that the Catch2 magic doesn't always play well with Nautilus.  See
// https://stackoverflow.com/q/59770581/1791919 for a discussion of a related problem.

// ================================================================================================

namespace detail {

auto load_isotopes()
{
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

TEST_CASE("format: MCNP partial zaid", "[entity_tag][format][MCNP]")
{
    using nautilus::entity_tag::EntityTag;
    using nautilus::entity_tag::from_MCNP_partial_zaid;
    using nautilus::entity_tag::invalid_mcnp_partial_zaid;
    using nautilus::entity_tag::to_MCNP_partial_zaid;

    // Normal nuclides
    const auto elements = detail::load_isotopes();
    for (const auto & element : elements) {
        const int Z = element.Z;
        for (const int A : element.As) {
            if ((Z == 95) && (A == 242)) {
                // skip Am-242, because it's a special case tested below
                break;
            }
            // MCNP allows the excited states m = 1, 2, 3, or 4
            for (int m = 0; m < 5; ++m) {
                EntityTag tag(Z, A, m);
                int partial_zaid = Z * 1000 + A;
                if (m != 0) {
                    partial_zaid += 300 + m * 100;
                }
                INFO(
                    "checking Z = " << Z << ", A = " << A << ", m = " << m
                                    << " (partial zaid = " << partial_zaid << ")");
                CHECK((from_MCNP_partial_zaid(partial_zaid) == tag));
                CHECK(to_MCNP_partial_zaid(tag) == partial_zaid);
            }
        }
    }

    // Special cases

    // Am-242g and Am-242m1 are swapped
    // m == 0
    constexpr EntityTag am242g_tag(95, 242);
    constexpr int am242g_zaid = 95642; // should be 95242 except it's swapped with Am-242m1
    CHECK((from_MCNP_partial_zaid(am242g_zaid) == am242g_tag));
    CHECK(to_MCNP_partial_zaid(am242g_tag) == am242g_zaid);
    // m = 1
    constexpr EntityTag am242m1_tag(95, 242, 1);
    constexpr int am242m1_zaid = 95242; // should be 95642 except it's swapped with Am-242g
    CHECK((from_MCNP_partial_zaid(am242m1_zaid) == am242m1_tag));
    CHECK(to_MCNP_partial_zaid(am242m1_tag) == am242m1_zaid);
    // m = 2
    constexpr EntityTag am242m2_tag(95, 242, 2);
    constexpr int am242m2_zaid = 95742; // follows the standard notation
    CHECK((from_MCNP_partial_zaid(am242m2_zaid) == am242m2_tag));
    CHECK(to_MCNP_partial_zaid(am242m2_tag) == am242m2_zaid);
    // m = 3
    constexpr EntityTag am242m3_tag(95, 242, 3);
    constexpr int am242m3_zaid = 95842; // follows the standard notation
    CHECK((from_MCNP_partial_zaid(am242m3_zaid) == am242m3_tag));
    CHECK(to_MCNP_partial_zaid(am242m3_tag) == am242m3_zaid);
    // m = 4
    constexpr EntityTag am242m4_tag(95, 242, 4);
    constexpr int am242m4_zaid = 95942; // follows the standard notation
    CHECK((from_MCNP_partial_zaid(am242m4_zaid) == am242m4_tag));
    CHECK(to_MCNP_partial_zaid(am242m4_tag) == am242m4_zaid);

    // m > 4 is not allowed
    // -- 200 <= A < 300 with m = 5 would lead to overflow that changes the Z value, so in general
    //    m > 4 is not permitted
    CHECK(to_MCNP_partial_zaid(EntityTag(6, 12, 5)) == invalid_mcnp_partial_zaid);
    CHECK((from_MCNP_partial_zaid(6812) == EntityTag(EntityTag::unknown)));
    CHECK(to_MCNP_partial_zaid(EntityTag(6, 12, 9)) == invalid_mcnp_partial_zaid);
    CHECK((from_MCNP_partial_zaid(6912) == EntityTag(EntityTag::unknown)));
    CHECK(to_MCNP_partial_zaid(EntityTag(47, 107, 5)) == invalid_mcnp_partial_zaid);
    CHECK((from_MCNP_partial_zaid(47907) == EntityTag(EntityTag::unknown)));

    // elementals
    CHECK((from_MCNP_partial_zaid(6000) == EntityTag(6, EntityTag::elemental)));
    CHECK(to_MCNP_partial_zaid(EntityTag(13, EntityTag::elemental)) == 13000);

    // particles
    CHECK(
        to_MCNP_partial_zaid(EntityTag(nautilus::entity_tag::names::proton)) ==
        invalid_mcnp_partial_zaid);
    CHECK(
        to_MCNP_partial_zaid(EntityTag(nautilus::entity_tag::names::muon)) ==
        invalid_mcnp_partial_zaid);

    // user
    CHECK(to_MCNP_partial_zaid(EntityTag(EntityTag::user, 0)) == invalid_mcnp_partial_zaid);

    // unknown
    CHECK(to_MCNP_partial_zaid(EntityTag(EntityTag::unknown)) == invalid_mcnp_partial_zaid);
}

// ================================================================================================

TEST_CASE("format: MCNP full zaid", "[entity_tag][format][MCNP]")
{
    using nautilus::entity_tag::EntityTag;
    using nautilus::entity_tag::from_MCNP_full_zaid;
    using nautilus::entity_tag::invalid_mcnp_full_zaid;
    using nautilus::entity_tag::to_MCNP_full_zaid;

    // Normal nuclides
    const auto elements = detail::load_isotopes();
    for (const auto & element : elements) {
        const int Z = element.Z;
        for (const int A : element.As) {
            if ((Z == 95) && (A == 242)) {
                // skip Am-242, because it's a special case tested below
                break;
            }
            // MCNP allows the excited states m = 1, 2, 3, or 4
            for (int m = 0; m < 5; ++m) {
                EntityTag tag(Z, A, m);
                int partial_zaid = Z * 1000 + A;
                if (m != 0) {
                    partial_zaid += 300 + m * 100;
                }
                std::string full_zaid = std::to_string(partial_zaid) + ".99c";
                INFO(
                    "checking Z = " << Z << ", A = " << A << ", m = " << m
                                    << " (partial zaid = " << full_zaid << ")");
                CHECK((from_MCNP_full_zaid(full_zaid) == tag));
                CHECK(to_MCNP_full_zaid(tag, "99c") == full_zaid);
            }
        }
    }

    // Special cases

    // Am-242g and Am-242m1 are swapped
    // m == 0
    constexpr EntityTag am242g_tag(95, 242);
    std::string am242g_zaid = "95642.00z"; // should be 95242 but swapped with Am-242m1
    CHECK((from_MCNP_full_zaid(am242g_zaid) == am242g_tag));
    CHECK(to_MCNP_full_zaid(am242g_tag, "00z") == am242g_zaid);
    // m = 1
    constexpr EntityTag am242m1_tag(95, 242, 1);
    std::string am242m1_zaid = "95242.11a"; // should be 95642 but swapped with Am-242g
    CHECK((from_MCNP_full_zaid(am242m1_zaid) == am242m1_tag));
    CHECK(to_MCNP_full_zaid(am242m1_tag, "11a") == am242m1_zaid);
    // m = 2
    constexpr EntityTag am242m2_tag(95, 242, 2);
    std::string am242m2_zaid = "95742.22b"; // follows the standard notation
    CHECK((from_MCNP_full_zaid(am242m2_zaid) == am242m2_tag));
    CHECK(to_MCNP_full_zaid(am242m2_tag, "22b") == am242m2_zaid);
    // m = 3
    constexpr EntityTag am242m3_tag(95, 242, 3);
    std::string am242m3_zaid = "95842.33c"; // follows the standard notation
    CHECK((from_MCNP_full_zaid(am242m3_zaid) == am242m3_tag));
    CHECK(to_MCNP_full_zaid(am242m3_tag, "33c") == am242m3_zaid);
    // m = 4
    constexpr EntityTag am242m4_tag(95, 242, 4);
    std::string am242m4_zaid = "95942.44d"; // follows the standard notation
    CHECK((from_MCNP_full_zaid(am242m4_zaid) == am242m4_tag));
    CHECK(to_MCNP_full_zaid(am242m4_tag, "44d") == am242m4_zaid);

    // m > 4 is not allowed
    // -- 200 <= A < 300 with m = 5 would lead to overflow that changes the Z value, so in general
    //    m > 4 is not permitted
    CHECK(to_MCNP_full_zaid(EntityTag(6, 12, 5), "abx") == invalid_mcnp_full_zaid);
    CHECK((from_MCNP_full_zaid("6812.abx") == EntityTag(EntityTag::unknown)));
    CHECK(to_MCNP_full_zaid(EntityTag(6, 12, 9), "nnc") == invalid_mcnp_full_zaid);
    CHECK((from_MCNP_full_zaid("6912.nnc") == EntityTag(EntityTag::unknown)));
    CHECK(to_MCNP_full_zaid(EntityTag(47, 107, 5), "zzz") == invalid_mcnp_full_zaid);
    CHECK((from_MCNP_full_zaid("47907.zzz") == EntityTag(EntityTag::unknown)));

    // elementals
    CHECK((from_MCNP_full_zaid("6000.nnc") == EntityTag(6, EntityTag::elemental)));
    CHECK(to_MCNP_full_zaid(EntityTag(13, EntityTag::elemental), "99j") == "13000.99j");

    // particles
    CHECK(
        to_MCNP_full_zaid(EntityTag(nautilus::entity_tag::names::proton), "00z") ==
        invalid_mcnp_full_zaid);
    CHECK(
        to_MCNP_full_zaid(EntityTag(nautilus::entity_tag::names::muon), "00z") ==
        invalid_mcnp_full_zaid);

    // user
    CHECK(to_MCNP_full_zaid(EntityTag(EntityTag::user, 0), "abx") == invalid_mcnp_full_zaid);

    // unknown
    CHECK(to_MCNP_full_zaid(EntityTag(EntityTag::unknown), "nnc") == invalid_mcnp_full_zaid);
}

// ================================================================================================

TEST_CASE("format: MCNP particle symbol", "[entity_tag][format][MCNP]")
{
    using nautilus::entity_tag::EntityTag;
    using nautilus::entity_tag::from_MCNP_particle_symbol;
    using nautilus::entity_tag::invalid_mcnp_particle_symbol;
    using nautilus::entity_tag::to_MCNP_particle_symbol;

    using namespace nautilus::entity_tag::names;

    // Nuclides
    // -- H-1 (EntityTag(1,1) -> 'H', but 'H' -> proton)
    CHECK((from_MCNP_particle_symbol('H') == EntityTag(proton)));
    CHECK(to_MCNP_particle_symbol(EntityTag(1, 1)) == 'H');
    // -- H-2
    CHECK((from_MCNP_particle_symbol('D') == EntityTag(1, 2)));
    CHECK(to_MCNP_particle_symbol(EntityTag(1, 2)) == 'D');
    // -- H-3
    CHECK((from_MCNP_particle_symbol('T') == EntityTag(1, 3)));
    CHECK(to_MCNP_particle_symbol(EntityTag(1, 3)) == 'T');
    // -- He-3
    CHECK((from_MCNP_particle_symbol('S') == EntityTag(2, 3)));
    CHECK(to_MCNP_particle_symbol(EntityTag(2, 3)) == 'S');
    // -- He-3
    CHECK((from_MCNP_particle_symbol('A') == EntityTag(2, 4)));
    CHECK(to_MCNP_particle_symbol(EntityTag(2, 4)) == 'A');
    // -- all other nuclides
    CHECK((from_MCNP_particle_symbol('#') == EntityTag(EntityTag::unknown)));
    CHECK(to_MCNP_particle_symbol(EntityTag(3, 7)) == '#');
    CHECK(to_MCNP_particle_symbol(EntityTag(4, 9)) == '#');
    CHECK(to_MCNP_particle_symbol(EntityTag(5, 11)) == '#');
    CHECK(to_MCNP_particle_symbol(EntityTag(118, 294)) == '#');
    CHECK(to_MCNP_particle_symbol(EntityTag(1, 4)) == '#');
    CHECK(to_MCNP_particle_symbol(EntityTag(2, 5)) == '#');

    // Particles
    CHECK((from_MCNP_particle_symbol('N') == EntityTag(neutron)));
    CHECK(to_MCNP_particle_symbol(EntityTag(neutron)) == 'N');
    CHECK((from_MCNP_particle_symbol('P') == EntityTag(photon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(photon)) == 'P');
    CHECK((from_MCNP_particle_symbol('E') == EntityTag(electron)));
    CHECK(to_MCNP_particle_symbol(EntityTag(electron)) == 'E');
    CHECK((from_MCNP_particle_symbol('|') == EntityTag(muon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(muon)) == '|');
    CHECK((from_MCNP_particle_symbol('Q') == EntityTag(antineutron)));
    CHECK(to_MCNP_particle_symbol(EntityTag(antineutron)) == 'Q');
    CHECK((from_MCNP_particle_symbol('U') == EntityTag(electron_neutrino)));
    CHECK(to_MCNP_particle_symbol(EntityTag(electron_neutrino)) == 'U');
    CHECK((from_MCNP_particle_symbol('V') == EntityTag(muon_neutrino)));
    CHECK(to_MCNP_particle_symbol(EntityTag(muon_neutrino)) == 'V');
    CHECK((from_MCNP_particle_symbol('F') == EntityTag(positron)));
    CHECK(to_MCNP_particle_symbol(EntityTag(positron)) == 'F');
    CHECK((from_MCNP_particle_symbol('H') == EntityTag(proton)));
    CHECK(to_MCNP_particle_symbol(EntityTag(proton)) == 'H');
    CHECK((from_MCNP_particle_symbol('L') == EntityTag(neutral_lambda_baryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(neutral_lambda_baryon)) == 'L');
    CHECK((from_MCNP_particle_symbol('+') == EntityTag(positive_sigma_baryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(positive_sigma_baryon)) == '+');
    CHECK((from_MCNP_particle_symbol('-') == EntityTag(negative_sigma_baryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(negative_sigma_baryon)) == '-');
    CHECK((from_MCNP_particle_symbol('X') == EntityTag(neutral_xi_baryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(neutral_xi_baryon)) == 'X');
    CHECK((from_MCNP_particle_symbol('Y') == EntityTag(negative_xi_baryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(negative_xi_baryon)) == 'Y');
    CHECK((from_MCNP_particle_symbol('O') == EntityTag(negative_omega_baryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(negative_omega_baryon)) == 'O');
    CHECK((from_MCNP_particle_symbol('!') == EntityTag(antimuon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(antimuon)) == '!');
    CHECK((from_MCNP_particle_symbol('<') == EntityTag(electron_antineutrino)));
    CHECK(to_MCNP_particle_symbol(EntityTag(electron_antineutrino)) == '<');
    CHECK((from_MCNP_particle_symbol('>') == EntityTag(muon_antineutrino)));
    CHECK(to_MCNP_particle_symbol(EntityTag(muon_antineutrino)) == '>');
    CHECK((from_MCNP_particle_symbol('G') == EntityTag(antiproton)));
    CHECK(to_MCNP_particle_symbol(EntityTag(antiproton)) == 'G');
    CHECK((from_MCNP_particle_symbol('/') == EntityTag(positive_pion)));
    CHECK(to_MCNP_particle_symbol(EntityTag(positive_pion)) == '/');
    CHECK((from_MCNP_particle_symbol('Z') == EntityTag(neutral_pion)));
    CHECK(to_MCNP_particle_symbol(EntityTag(neutral_pion)) == 'Z');
    CHECK((from_MCNP_particle_symbol('K') == EntityTag(positive_kaon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(positive_kaon)) == 'K');
    CHECK((from_MCNP_particle_symbol('%') == EntityTag(short_kaon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(short_kaon)) == '%');
    CHECK((from_MCNP_particle_symbol('^') == EntityTag(long_kaon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(long_kaon)) == '^');
    CHECK((from_MCNP_particle_symbol('B') == EntityTag(neutral_lambda_antibaryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(neutral_lambda_antibaryon)) == 'B');
    CHECK((from_MCNP_particle_symbol('_') == EntityTag(negative_sigma_antibaryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(negative_sigma_antibaryon)) == '_');
    CHECK((from_MCNP_particle_symbol('~') == EntityTag(positive_sigma_antibaryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(positive_sigma_antibaryon)) == '~');
    CHECK((from_MCNP_particle_symbol('C') == EntityTag(neutral_xi_antibaryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(neutral_xi_antibaryon)) == 'C');
    CHECK((from_MCNP_particle_symbol('W') == EntityTag(positive_xi_antibaryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(positive_xi_antibaryon)) == 'W');
    CHECK((from_MCNP_particle_symbol('@') == EntityTag(positive_omega_antibaryon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(positive_omega_antibaryon)) == '@');
    CHECK((from_MCNP_particle_symbol('*') == EntityTag(negative_pion)));
    CHECK(to_MCNP_particle_symbol(EntityTag(negative_pion)) == '*');
    CHECK((from_MCNP_particle_symbol('?') == EntityTag(negative_kaon)));
    CHECK(to_MCNP_particle_symbol(EntityTag(negative_kaon)) == '?');
}

// ================================================================================================
