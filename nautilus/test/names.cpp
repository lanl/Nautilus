#include "names.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("Particle names", "[names]")
{
    using namespace nautilus::names;

    auto f1 = [](const std::size_t index, const std::string_view name) {
        return particles[index].get_name() == name;
    };
    CHECK(f1(photon, "photon"));
    CHECK(f1(electron, "electron"));
    CHECK(f1(positron, "positron"));
    CHECK(f1(electron_neutrino, "electron neutrino"));
    CHECK(f1(electron_antineutrino, "electron antineutrino"));
    CHECK(f1(muon, "muon"));
    CHECK(f1(antimuon, "antimuon"));
    CHECK(f1(muon_neutrino, "muon neutrino"));
    CHECK(f1(muon_antineutrino, "muon antineutrino"));
    CHECK(f1(neutral_pion, "neutral pion"));
    CHECK(f1(positive_pion, "positive pion"));
    CHECK(f1(negative_pion, "negative pion"));
    CHECK(f1(short_kaon, "short kaon"));
    CHECK(f1(long_kaon, "long kaon"));
    CHECK(f1(positive_kaon, "positive kaon"));
    CHECK(f1(negative_kaon, "negative kaon"));
    CHECK(f1(neutron, "neutron"));
    CHECK(f1(antineutron, "antineutron"));
    CHECK(f1(proton, "proton"));
    CHECK(f1(antiproton, "antiproton"));
    CHECK(f1(neutral_lambda_baryon, "neutral lambda baryon"));
    CHECK(f1(neutral_lambda_antibaryon, "neutral lambda antibaryon"));
    CHECK(f1(positive_sigma_baryon, "positive sigma baryon"));
    CHECK(f1(negative_sigma_antibaryon, "negative sigma antibaryon"));
    CHECK(f1(negative_sigma_baryon, "negative sigma baryon"));
    CHECK(f1(positive_sigma_antibaryon, "positive sigma antibaryon"));
    CHECK(f1(neutral_xi_baryon, "neutral xi baryon"));
    CHECK(f1(neutral_xi_antibaryon, "neutral xi antibaryon"));
    CHECK(f1(negative_xi_baryon, "negative xi baryon"));
    CHECK(f1(positive_xi_antibaryon, "positive xi antibaryon"));
    CHECK(f1(negative_omega_baryon, "negative omega baryon"));
    CHECK(f1(positive_omega_antibaryon, "positive omega antibaryon"));

    auto f2 = [](const std::size_t index, const std::string_view name) {
        return get_particle_name(index) == name;
    };
    CHECK(f2(photon, "photon"));
    CHECK(f2(electron, "electron"));
    CHECK(f2(positron, "positron"));
    CHECK(f2(electron_neutrino, "electron neutrino"));
    CHECK(f2(electron_antineutrino, "electron antineutrino"));
    CHECK(f2(muon, "muon"));
    CHECK(f2(antimuon, "antimuon"));
    CHECK(f2(muon_neutrino, "muon neutrino"));
    CHECK(f2(muon_antineutrino, "muon antineutrino"));
    CHECK(f2(neutral_pion, "neutral pion"));
    CHECK(f2(positive_pion, "positive pion"));
    CHECK(f2(negative_pion, "negative pion"));
    CHECK(f2(short_kaon, "short kaon"));
    CHECK(f2(long_kaon, "long kaon"));
    CHECK(f2(positive_kaon, "positive kaon"));
    CHECK(f2(negative_kaon, "negative kaon"));
    CHECK(f2(neutron, "neutron"));
    CHECK(f2(antineutron, "antineutron"));
    CHECK(f2(proton, "proton"));
    CHECK(f2(antiproton, "antiproton"));
    CHECK(f2(neutral_lambda_baryon, "neutral lambda baryon"));
    CHECK(f2(neutral_lambda_antibaryon, "neutral lambda antibaryon"));
    CHECK(f2(positive_sigma_baryon, "positive sigma baryon"));
    CHECK(f2(negative_sigma_antibaryon, "negative sigma antibaryon"));
    CHECK(f2(negative_sigma_baryon, "negative sigma baryon"));
    CHECK(f2(positive_sigma_antibaryon, "positive sigma antibaryon"));
    CHECK(f2(neutral_xi_baryon, "neutral xi baryon"));
    CHECK(f2(neutral_xi_antibaryon, "neutral xi antibaryon"));
    CHECK(f2(negative_xi_baryon, "negative xi baryon"));
    CHECK(f2(positive_xi_antibaryon, "positive xi antibaryon"));
    CHECK(f2(negative_omega_baryon, "negative omega baryon"));
    CHECK(f2(positive_omega_antibaryon, "positive omega antibaryon"));

    auto f3 = [](const std::size_t index, const std::string_view name) {
        return particles[index].get_name(Particle::Standard::alternate) == name;
    };
    CHECK(f3(photon, "photon"));
    CHECK(f3(electron, "electron"));
    CHECK(f3(positron, "positron"));
    CHECK(f3(electron_neutrino, "electron neutrino"));
    CHECK(f3(electron_antineutrino, "electron antineutrino"));
    CHECK(f3(muon, "muon"));
    CHECK(f3(antimuon, "antimuon"));
    CHECK(f3(muon_neutrino, "muon neutrino"));
    CHECK(f3(muon_antineutrino, "muon antineutrino"));
    CHECK(f3(neutral_pion, "neutral pion"));
    CHECK(f3(positive_pion, "positive pion"));
    CHECK(f3(negative_pion, "negative pion"));
    CHECK(f3(short_kaon, "short kaon"));
    CHECK(f3(long_kaon, "long kaon"));
    CHECK(f3(positive_kaon, "positive kaon"));
    CHECK(f3(negative_kaon, "negative kaon"));
    CHECK(f3(neutron, "neutron"));
    CHECK(f3(antineutron, "antineutron"));
    CHECK(f3(proton, "proton"));
    CHECK(f3(antiproton, "antiproton"));
    CHECK(f3(neutral_lambda_baryon, "neutral lambda baryon"));
    CHECK(f3(neutral_lambda_antibaryon, "antiparticle of the neutral lambda baryon"));
    CHECK(f3(positive_sigma_baryon, "positive sigma baryon"));
    CHECK(f3(negative_sigma_antibaryon, "antiparticle of the positive sigma baryon"));
    CHECK(f3(negative_sigma_baryon, "negative sigma baryon"));
    CHECK(f3(positive_sigma_antibaryon, "antiparticle of the negative sigma baryon"));
    CHECK(f3(neutral_xi_baryon, "neutral xi baryon"));
    CHECK(f3(neutral_xi_antibaryon, "antiparticle of the neutral xi baryon"));
    CHECK(f3(negative_xi_baryon, "negative xi baryon"));
    CHECK(f3(positive_xi_antibaryon, "antiparticle of the negative xi baryon"));
    CHECK(f3(negative_omega_baryon, "negative omega baryon"));
    CHECK(f3(positive_omega_antibaryon, "antiparticle of the negative omega baryon"));

    auto f4 = [](const std::size_t index, const std::string_view name) {
        return get_particle_name(index, Particle::Standard::alternate) == name;
    };
    CHECK(f4(photon, "photon"));
    CHECK(f4(electron, "electron"));
    CHECK(f4(positron, "positron"));
    CHECK(f4(electron_neutrino, "electron neutrino"));
    CHECK(f4(electron_antineutrino, "electron antineutrino"));
    CHECK(f4(muon, "muon"));
    CHECK(f4(antimuon, "antimuon"));
    CHECK(f4(muon_neutrino, "muon neutrino"));
    CHECK(f4(muon_antineutrino, "muon antineutrino"));
    CHECK(f4(neutral_pion, "neutral pion"));
    CHECK(f4(positive_pion, "positive pion"));
    CHECK(f4(negative_pion, "negative pion"));
    CHECK(f4(short_kaon, "short kaon"));
    CHECK(f4(long_kaon, "long kaon"));
    CHECK(f4(positive_kaon, "positive kaon"));
    CHECK(f4(negative_kaon, "negative kaon"));
    CHECK(f4(neutron, "neutron"));
    CHECK(f4(antineutron, "antineutron"));
    CHECK(f4(proton, "proton"));
    CHECK(f4(antiproton, "antiproton"));
    CHECK(f4(neutral_lambda_baryon, "neutral lambda baryon"));
    CHECK(f4(neutral_lambda_antibaryon, "antiparticle of the neutral lambda baryon"));
    CHECK(f4(positive_sigma_baryon, "positive sigma baryon"));
    CHECK(f4(negative_sigma_antibaryon, "antiparticle of the positive sigma baryon"));
    CHECK(f4(negative_sigma_baryon, "negative sigma baryon"));
    CHECK(f4(positive_sigma_antibaryon, "antiparticle of the negative sigma baryon"));
    CHECK(f4(neutral_xi_baryon, "neutral xi baryon"));
    CHECK(f4(neutral_xi_antibaryon, "antiparticle of the neutral xi baryon"));
    CHECK(f4(negative_xi_baryon, "negative xi baryon"));
    CHECK(f4(positive_xi_antibaryon, "antiparticle of the negative xi baryon"));
    CHECK(f4(negative_omega_baryon, "negative omega baryon"));
    CHECK(f4(positive_omega_antibaryon, "antiparticle of the negative omega baryon"));
}

// ================================================================================================

TEST_CASE("Particle symbols", "[names]")
{
    using namespace nautilus::names;

    auto f1 = [](const std::size_t index, const std::string_view name) {
        return particles[index].get_symbol() == name;
    };
    CHECK(f1(photon, "\u03B3"));
    CHECK(f1(electron, "e\u207B"));
    CHECK(f1(positron, "e\u207A"));
    CHECK(f1(electron_neutrino, "\u03BD\u2091"));
    CHECK(f1(electron_antineutrino, "\u03BD\u0304\u2091"));
    CHECK(f1(muon, "\u03Bc\u207B"));
    CHECK(f1(antimuon, "\u03BC\u0304\u207A"));
    CHECK(f1(muon_neutrino, "\u03BD\u2098"));
    CHECK(f1(muon_antineutrino, "\u03BD\u0304\2098"));
    CHECK(f1(neutral_pion, "\u03C0\u2070"));
    CHECK(f1(positive_pion, "\u03C0\u207A"));
    CHECK(f1(negative_pion, "\u03C0\u207B"));
    CHECK(f1(short_kaon, "K\u2070\u209B"));
    CHECK(f1(long_kaon, "K\u2070\u2097"));
    CHECK(f1(positive_kaon, "K\u207A"));
    CHECK(f1(negative_kaon, "K\u207B"));
    CHECK(f1(neutron, "n"));
    CHECK(f1(antineutron, "n\u0304"));
    CHECK(f1(proton, "p"));
    CHECK(f1(antiproton, "p\u0304"));
    CHECK(f1(neutral_lambda_baryon, "\u039B\u2070"));
    CHECK(f1(neutral_lambda_antibaryon, "\u039B\u0304\u2070"));
    CHECK(f1(positive_sigma_baryon, "\u03A3\u207A"));
    CHECK(f1(negative_sigma_antibaryon, "\u03A3\u0304\u207B"));
    CHECK(f1(negative_sigma_baryon, "\u03A3\u207B"));
    CHECK(f1(positive_sigma_antibaryon, "\u03A3\u0304\u207A"));
    CHECK(f1(neutral_xi_baryon, "\u039E\u2070"));
    CHECK(f1(neutral_xi_antibaryon, "\u039E\u0304\u2070"));
    CHECK(f1(negative_xi_baryon, "\u039E\u207B"));
    CHECK(f1(positive_xi_antibaryon, "\u039E\u0304\u207A"));
    CHECK(f1(negative_omega_baryon, "\u03A9\u207B"));
    CHECK(f1(positive_omega_antibaryon, "\u03A9\u0304\u207A"));

    auto f2 = [](const std::size_t index, const std::string_view name) {
        return get_particle_symbol(index) == name;
    };
    CHECK(f2(photon, "\u03B3"));
    CHECK(f2(electron, "e\u207B"));
    CHECK(f2(positron, "e\u207A"));
    CHECK(f2(electron_neutrino, "\u03BD\u2091"));
    CHECK(f2(electron_antineutrino, "\u03BD\u0304\u2091"));
    CHECK(f2(muon, "\u03Bc\u207B"));
    CHECK(f2(antimuon, "\u03BC\u0304\u207A"));
    CHECK(f2(muon_neutrino, "\u03BD\u2098"));
    CHECK(f2(muon_antineutrino, "\u03BD\u0304\2098"));
    CHECK(f2(neutral_pion, "\u03C0\u2070"));
    CHECK(f2(positive_pion, "\u03C0\u207A"));
    CHECK(f2(negative_pion, "\u03C0\u207B"));
    CHECK(f2(short_kaon, "K\u2070\u209B"));
    CHECK(f2(long_kaon, "K\u2070\u2097"));
    CHECK(f2(positive_kaon, "K\u207A"));
    CHECK(f2(negative_kaon, "K\u207B"));
    CHECK(f2(neutron, "n"));
    CHECK(f2(antineutron, "n\u0304"));
    CHECK(f2(proton, "p"));
    CHECK(f2(antiproton, "p\u0304"));
    CHECK(f2(neutral_lambda_baryon, "\u039B\u2070"));
    CHECK(f2(neutral_lambda_antibaryon, "\u039B\u0304\u2070"));
    CHECK(f2(positive_sigma_baryon, "\u03A3\u207A"));
    CHECK(f2(negative_sigma_antibaryon, "\u03A3\u0304\u207B"));
    CHECK(f2(negative_sigma_baryon, "\u03A3\u207B"));
    CHECK(f2(positive_sigma_antibaryon, "\u03A3\u0304\u207A"));
    CHECK(f2(neutral_xi_baryon, "\u039E\u2070"));
    CHECK(f2(neutral_xi_antibaryon, "\u039E\u0304\u2070"));
    CHECK(f2(negative_xi_baryon, "\u039E\u207B"));
    CHECK(f2(positive_xi_antibaryon, "\u039E\u0304\u207A"));
    CHECK(f2(negative_omega_baryon, "\u03A9\u207B"));
    CHECK(f2(positive_omega_antibaryon, "\u03A9\u0304\u207A"));

    // Symbols are not available for the "alternate" format.
}

// ================================================================================================

