#include "names.hpp"

#include <catch2/catch_test_macros.hpp>

// ================================================================================================

TEST_CASE("Name indices", "[names]")
{
    using namespace nautilus::names;
    CHECK(particles[photon                      ].get_name() == "photon"                    );
    CHECK(particles[electron                    ].get_name() == "electron"                  );
    CHECK(particles[positron                    ].get_name() == "positron"                  );
    CHECK(particles[electron_neutrino           ].get_name() == "electron neutrino"         );
    CHECK(particles[electron_antineutrino       ].get_name() == "electron antineutrino"     );
    CHECK(particles[muon                        ].get_name() == "muon"                      );
    CHECK(particles[antimuon                    ].get_name() == "antimuon"                  );
    CHECK(particles[muon_neutrino               ].get_name() == "muon neutrino"             );
    CHECK(particles[muon_antineutrino           ].get_name() == "muon antineutrino"         );
    CHECK(particles[neutral_pion                ].get_name() == "neutral pion"              );
    CHECK(particles[positive_pion               ].get_name() == "positive pion"             );
    CHECK(particles[negative_pion               ].get_name() == "negative pion"             );
    CHECK(particles[short_kaon                  ].get_name() == "short kaon"                );
    CHECK(particles[long_kaon                   ].get_name() == "long kaon"                 );
    CHECK(particles[positive_kaon               ].get_name() == "positive kaon"             );
    CHECK(particles[negative_kaon               ].get_name() == "negative kaon"             );
    CHECK(particles[neutron                     ].get_name() == "neutron"                   );
    CHECK(particles[antineutron                 ].get_name() == "antineutron"               );
    CHECK(particles[proton                      ].get_name() == "proton"                    );
    CHECK(particles[antiproton                  ].get_name() == "antiproton"                );
    CHECK(particles[neutral_lambda_baryon       ].get_name() == "neutral lambda baryon"     );
    CHECK(particles[neutral_lambda_antibaryon   ].get_name() == "neutral lambda antibaryon" );
    CHECK(particles[positive_sigma_baryon       ].get_name() == "positive sigma baryon"     );
    CHECK(particles[negative_sigma_antibaryon   ].get_name() == "negative sigma antibaryon" );
    CHECK(particles[negative_sigma_baryon       ].get_name() == "negative sigma baryon"     );
    CHECK(particles[positive_sigma_antibaryon   ].get_name() == "positive sigma antibaryon" );
    CHECK(particles[neutral_xi_baryon           ].get_name() == "neutral xi baryon"         );
    CHECK(particles[neutral_xi_antibaryon       ].get_name() == "neutral xi antibaryon"     );
    CHECK(particles[negative_xi_baryon          ].get_name() == "negative xi baryon"        );
    CHECK(particles[positive_xi_antibaryon      ].get_name() == "positive xi antibaryon"    );
    CHECK(particles[negative_omega_baryon       ].get_name() == "negative omega baryon"     );
    CHECK(particles[positive_omega_antibaryon   ].get_name() == "positive omega antibaryon" );
}

