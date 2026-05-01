#include "names.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("particle names", "[names]")
{
    using namespace nautilus::entity_tag::names;

    auto f_pdg = [](const particle_index_t index, const std::string_view name) {
        return Particles::get_name(index) == name;
    };
    CHECK(f_pdg(photon, "photon"));
    CHECK(f_pdg(electron, "electron"));
    CHECK(f_pdg(positron, "positron"));
    CHECK(f_pdg(electron_neutrino, "electron neutrino"));
    CHECK(f_pdg(electron_antineutrino, "electron antineutrino"));
    CHECK(f_pdg(muon, "muon"));
    CHECK(f_pdg(antimuon, "antimuon"));
    CHECK(f_pdg(muon_neutrino, "muon neutrino"));
    CHECK(f_pdg(muon_antineutrino, "muon antineutrino"));
    CHECK(f_pdg(neutral_pion, "neutral pion"));
    CHECK(f_pdg(positive_pion, "positive pion"));
    CHECK(f_pdg(negative_pion, "negative pion"));
    CHECK(f_pdg(short_kaon, "short kaon"));
    CHECK(f_pdg(long_kaon, "long kaon"));
    CHECK(f_pdg(positive_kaon, "positive kaon"));
    CHECK(f_pdg(negative_kaon, "negative kaon"));
    CHECK(f_pdg(neutron, "neutron"));
    CHECK(f_pdg(antineutron, "antineutron"));
    CHECK(f_pdg(proton, "proton"));
    CHECK(f_pdg(antiproton, "antiproton"));
    CHECK(f_pdg(neutral_lambda_baryon, "neutral lambda baryon"));
    CHECK(f_pdg(neutral_lambda_antibaryon, "neutral lambda antibaryon"));
    CHECK(f_pdg(positive_sigma_baryon, "positive sigma baryon"));
    CHECK(f_pdg(negative_sigma_antibaryon, "negative sigma antibaryon"));
    CHECK(f_pdg(negative_sigma_baryon, "negative sigma baryon"));
    CHECK(f_pdg(positive_sigma_antibaryon, "positive sigma antibaryon"));
    CHECK(f_pdg(neutral_xi_baryon, "neutral xi baryon"));
    CHECK(f_pdg(neutral_xi_antibaryon, "neutral xi antibaryon"));
    CHECK(f_pdg(negative_xi_baryon, "negative xi baryon"));
    CHECK(f_pdg(positive_xi_antibaryon, "positive xi antibaryon"));
    CHECK(f_pdg(negative_omega_baryon, "negative omega baryon"));
    CHECK(f_pdg(positive_omega_antibaryon, "positive omega antibaryon"));

    auto f_alt = [](const particle_index_t index, const std::string_view name) {
        return Particles::get_name(index, Particles::Standard::alternate) == name;
    };
    CHECK(f_alt(photon, "photon"));
    CHECK(f_alt(electron, "electron"));
    CHECK(f_alt(positron, "positron"));
    CHECK(f_alt(electron_neutrino, "electron neutrino"));
    CHECK(f_alt(electron_antineutrino, "electron antineutrino"));
    CHECK(f_alt(muon, "muon"));
    CHECK(f_alt(antimuon, "antimuon"));
    CHECK(f_alt(muon_neutrino, "muon neutrino"));
    CHECK(f_alt(muon_antineutrino, "muon antineutrino"));
    CHECK(f_alt(neutral_pion, "neutral pion"));
    CHECK(f_alt(positive_pion, "positive pion"));
    CHECK(f_alt(negative_pion, "negative pion"));
    CHECK(f_alt(short_kaon, "short kaon"));
    CHECK(f_alt(long_kaon, "long kaon"));
    CHECK(f_alt(positive_kaon, "positive kaon"));
    CHECK(f_alt(negative_kaon, "negative kaon"));
    CHECK(f_alt(neutron, "neutron"));
    CHECK(f_alt(antineutron, "antineutron"));
    CHECK(f_alt(proton, "proton"));
    CHECK(f_alt(antiproton, "antiproton"));
    CHECK(f_alt(neutral_lambda_baryon, "neutral lambda baryon"));
    CHECK(f_alt(neutral_lambda_antibaryon, "antiparticle of the neutral lambda baryon"));
    CHECK(f_alt(positive_sigma_baryon, "positive sigma baryon"));
    CHECK(f_alt(negative_sigma_antibaryon, "antiparticle of the positive sigma baryon"));
    CHECK(f_alt(negative_sigma_baryon, "negative sigma baryon"));
    CHECK(f_alt(positive_sigma_antibaryon, "antiparticle of the negative sigma baryon"));
    CHECK(f_alt(neutral_xi_baryon, "neutral xi baryon"));
    CHECK(f_alt(neutral_xi_antibaryon, "antiparticle of the neutral xi baryon"));
    CHECK(f_alt(negative_xi_baryon, "negative xi baryon"));
    CHECK(f_alt(positive_xi_antibaryon, "antiparticle of the negative xi baryon"));
    CHECK(f_alt(negative_omega_baryon, "negative omega baryon"));
    CHECK(f_alt(positive_omega_antibaryon, "antiparticle of the negative omega baryon"));
}

// ================================================================================================

TEST_CASE("particle symbols", "[names]")
{
    using namespace nautilus::entity_tag::names;

    // Symbols are not available for the "alternate" format.
    auto f_pdg = [](const particle_index_t index, const std::string_view name) {
        return Particles::get_symbol(index) == name;
    };
    CHECK(f_pdg(photon, "\u03B3"));
    CHECK(f_pdg(electron, "e\u207B"));
    CHECK(f_pdg(positron, "e\u207A"));
    CHECK(f_pdg(electron_neutrino, "\u03BD\u2091"));
    CHECK(f_pdg(electron_antineutrino, "\u03BD\u0304\u2091"));
    CHECK(f_pdg(muon, "\u03Bc\u207B"));
    CHECK(f_pdg(antimuon, "\u03BC\u0304\u207A"));
    CHECK(f_pdg(muon_neutrino, "\u03BD\u2098"));
    CHECK(f_pdg(muon_antineutrino, "\u03BD\u0304\u2098"));
    CHECK(f_pdg(neutral_pion, "\u03C0\u2070"));
    CHECK(f_pdg(positive_pion, "\u03C0\u207A"));
    CHECK(f_pdg(negative_pion, "\u03C0\u207B"));
    CHECK(f_pdg(short_kaon, "K\u2070\u209B"));
    CHECK(f_pdg(long_kaon, "K\u2070\u2097"));
    CHECK(f_pdg(positive_kaon, "K\u207A"));
    CHECK(f_pdg(negative_kaon, "K\u207B"));
    CHECK(f_pdg(neutron, "n"));
    CHECK(f_pdg(antineutron, "n\u0304"));
    CHECK(f_pdg(proton, "p"));
    CHECK(f_pdg(antiproton, "p\u0304"));
    CHECK(f_pdg(neutral_lambda_baryon, "\u039B\u2070"));
    CHECK(f_pdg(neutral_lambda_antibaryon, "\u039B\u0304\u2070"));
    CHECK(f_pdg(positive_sigma_baryon, "\u03A3\u207A"));
    CHECK(f_pdg(negative_sigma_antibaryon, "\u03A3\u0304\u207B"));
    CHECK(f_pdg(negative_sigma_baryon, "\u03A3\u207B"));
    CHECK(f_pdg(positive_sigma_antibaryon, "\u03A3\u0304\u207A"));
    CHECK(f_pdg(neutral_xi_baryon, "\u039E\u2070"));
    CHECK(f_pdg(neutral_xi_antibaryon, "\u039E\u0304\u2070"));
    CHECK(f_pdg(negative_xi_baryon, "\u039E\u207B"));
    CHECK(f_pdg(positive_xi_antibaryon, "\u039E\u0304\u207A"));
    CHECK(f_pdg(negative_omega_baryon, "\u03A9\u207B"));
    CHECK(f_pdg(positive_omega_antibaryon, "\u03A9\u0304\u207A"));

    std::size_t max_size;
    printf("particle names (PDG):\n");
    max_size = 0;
    for (std::size_t n = 0; n < Particles::count; ++n) {
        const auto name = Particles::get_name(n);
        const auto size = name.size();
        printf("[%2zu]  (%2zu)  \"%s\"\n", n, size, name.data());
        max_size = std::max(size, max_size);
    }
    printf("length of longest string (byte count, not glyph count) = %zu\n", max_size);
    printf("particle names (alternate):\n");
    max_size = 0;
    for (std::size_t n = 0; n < Particles::count; ++n) {
        const auto name = Particles::get_name(n, Particles::Standard::alternate);
        const auto size = name.size();
        printf("[%2zu]  (%2zu)  \"%s\"\n", n, size, name.data());
        max_size = std::max(size, max_size);
    }
    printf("length of longest string (byte count, not glyph count) = %zu\n", max_size);
    printf("particle symbols:\n");
    max_size = 0;
    for (std::size_t n = 0; n < Particles::count; ++n) {
        const auto symbol = Particles::get_symbol(n);
        const auto size = symbol.size();
        printf("[%2zu]  (%2zu)  \"%s\"\n", n, size, symbol.data());
        max_size = std::max(size, max_size);
    }
    printf("length of longest string (byte count, not glyph count) = %zu\n", max_size);
}

// ================================================================================================

TEST_CASE("nuclide names and symbols", "[names]")
{
    using namespace nautilus::entity_tag::names;

    constexpr std::size_t H_Z = 1;
    CHECK(Nuclides::get_name(H_Z) == "hydrogen");
    CHECK(Nuclides::get_symbol(H_Z) == "H");

    constexpr std::size_t He_Z = 2;
    CHECK(Nuclides::find_index("He") == He_Z);
    CHECK(Nuclides::find_index("helium") == He_Z);
    CHECK(Nuclides::get_name(He_Z) == "helium");
    CHECK(Nuclides::get_name(He_Z, Nuclides::Standard::IUPAC) == "helium");
    CHECK(Nuclides::get_name(He_Z, Nuclides::Standard::American) == "helium");
    CHECK(Nuclides::get_name(He_Z, Nuclides::Standard::British) == "helium");
    CHECK(Nuclides::get_name(He_Z, Nuclides::Standard::Canadian) == "helium");
    CHECK(Nuclides::get_symbol(He_Z) == "He");

    constexpr std::size_t Al_Z = 13;
    CHECK(Nuclides::find_index("Al") == Al_Z);
    CHECK(Nuclides::find_index("aluminium") == Al_Z);
    CHECK(Nuclides::find_index("aluminum") == Al_Z);
    CHECK(Nuclides::get_name(Al_Z) == "aluminium");
    CHECK(Nuclides::get_name(Al_Z, Nuclides::Standard::American) == "aluminum");
    CHECK(Nuclides::get_name(Al_Z, Nuclides::Standard::British) == "aluminium");
    CHECK(Nuclides::get_name(Al_Z, Nuclides::Standard::Canadian) == "aluminum");
    CHECK(Nuclides::get_symbol(Al_Z) == "Al");

    constexpr std::size_t S_Z = 16;
    CHECK(Nuclides::find_index("S") == S_Z);
    CHECK(Nuclides::find_index("sulfur") == S_Z);
    CHECK(Nuclides::find_index("sulphur") == S_Z);
    CHECK(Nuclides::get_name(S_Z) == "sulfur");
    CHECK(Nuclides::get_name(S_Z, Nuclides::Standard::IUPAC) == "sulfur");
    CHECK(Nuclides::get_name(S_Z, Nuclides::Standard::American) == "sulfur");
    CHECK(Nuclides::get_name(S_Z, Nuclides::Standard::British) == "sulphur");
    CHECK(Nuclides::get_name(S_Z, Nuclides::Standard::Canadian) == "sulfur");
    CHECK(Nuclides::get_symbol(S_Z) == "S");

    constexpr std::size_t Cs_Z = 55;
    CHECK(Nuclides::find_index("Cs") == Cs_Z);
    CHECK(Nuclides::find_index("caesium") == Cs_Z);
    CHECK(Nuclides::find_index("cesium") == Cs_Z);
    CHECK(Nuclides::get_name(Cs_Z) == "caesium");
    CHECK(Nuclides::get_name(Cs_Z, Nuclides::Standard::IUPAC) == "caesium");
    CHECK(Nuclides::get_name(Cs_Z, Nuclides::Standard::American) == "cesium");
    CHECK(Nuclides::get_name(Cs_Z, Nuclides::Standard::British) == "caesium");
    CHECK(Nuclides::get_name(Cs_Z, Nuclides::Standard::Canadian) == "caesium");
    CHECK(Nuclides::get_symbol(Cs_Z) == "Cs");

    constexpr std::size_t Pr_Z = 59;
    CHECK(Nuclides::get_name(Pr_Z) == "praseodymium");
    CHECK(Nuclides::get_symbol(Pr_Z) == "Pr");

    constexpr std::size_t Og_Z = 118;
    CHECK(Nuclides::get_name(Og_Z) == "oganesson");
    CHECK(Nuclides::get_symbol(Og_Z) == "Og");
}
