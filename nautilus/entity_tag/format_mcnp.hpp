#ifndef NAUTILUS_FORMAT_MCNP_HPP
#define NAUTILUS_FORMAT_MCNP_HPP

#include <algorithm>
#include <cmath>
#include <limits>

#include "nautilus/entity_tag/entity_tag.hpp"
#include "nautilus/entity_tag/string_processing.hpp"

namespace nautilus::entity_tag {

// ================================================================================================
// MCNP partial zaid

constexpr int invalid_mcnp_partial_zaid = -1;

inline int to_MCNP_partial_zaid(const EntityTag tag)
{
    if (!tag.is_nuclide()) {
        return invalid_mcnp_partial_zaid;
    } else {
        // Get the values needed to assemble the partial zaid
        const auto Z = tag.get_atomic_number();
        if (tag.is_elemental()) {
            return Z * 1000; // A = 0 for elementals
        }
        const auto A = tag.get_atomic_mass_number();
        auto m = tag.get_metastable_index();
        if ((Z == 95) && (A == 242)) {
            // Am-242 swaps the m values for the ground state and the first metastable state
            if (m == 0) {
                m = 1;
            } else if (m == 1) {
                m = 0;
            }
        }
        // assemble the value
        auto partial_zaid = Z * 1000 + A;
        if (m > 4) { // MCNP partial zaid does not support m > 4
            return invalid_mcnp_partial_zaid;
        }
        if (m != 0) {
            partial_zaid += 300 + m * 100;
        }
        return partial_zaid;
    }
}

inline EntityTag from_MCNP_partial_zaid(const int partial_zaid)
{
    // Split into Z and A
    auto A = partial_zaid % 1000;
    const auto Z = partial_zaid / 1000;
    if (Z == 0) {
        return EntityTag(EntityTag::unknown);
    }
    // Check for metastable states
    int m = 0;
    // -- The heaviest known nuclide is a little below 300, and if there's a metastable state then
    //    we know A will be greater than 400 (300 + m * 100 + A, with m >= 1 and A >= 1)
    if (A > 400) {
        // Approximate A = f(Z)
        // -- Based on fitting hundreds of isotopes (across elements 1 - 118) found on Wikipedia.
        // -- Accurate to within +/- 10 for all isotopes tested.  The rounding strategy below
        //    requires that it be strictly within +/- 50 for all isotopes.
        // -- Conveniently, the cubic fit always gives non-negative values for A and for N = A - Z
        //    (not sure if that's important, but it's nice to have).
        auto f = [](const int Z) {
            const auto Z2 = Z * Z;
            const auto Z3 = Z * Z2;
            return -0.0000711325 * Z3 + 0.0147356 * Z2 + 1.75397 * Z + 2.04063;
        };
        m = std::round(0.01 * (A - 300 - f(Z)));
        assert(m > 0);        // sanity check: if A > 400 then we must have an excited state
        assert(m < 5);        // MCNP format only supports up to m == 4
        A -= (300 + m * 100); // remove the metastable correction from the atomic mass number
        assert(A >= Z);       // sanity check: cannot have negative neutrons
    }
    // -- special cases: Am-242g and Am-242m1 are swapped
    if ((Z == 95) && (A == 242)) {
        if (m == 0) {
            m = 1;
        } else if (m == 1) {
            m = 0;
        }
    }
    // construct the EntityTag
    if /* elemental */ (A == 0) {
        return EntityTag(Z, EntityTag::elemental);
    } else if /* ground state */ (m == 0) {
        return EntityTag(Z, A);
    } else /* excited state */ {
        return EntityTag(Z, A, m);
    }
}

// ================================================================================================
// MCNP full zaid

const std::string invalid_mcnp_full_zaid = "unknown";

std::string to_MCNP_full_zaid(EntityTag tag, const std::string_view library)
{
    if (tag.is_unknown()) {
        return invalid_mcnp_full_zaid;
    }
    const auto partial_zaid = to_MCNP_partial_zaid(tag);
    if (partial_zaid == invalid_mcnp_partial_zaid) {
        return invalid_mcnp_full_zaid;
    }
    std::string zaid = std::to_string(partial_zaid);
    zaid.append(".");
    zaid.append(library);
    return zaid;
}

inline EntityTag from_MCNP_full_zaid(const std::string_view sv)
{
    if (sv == invalid_mcnp_full_zaid) {
        return EntityTag(EntityTag::unknown);
    }
    return from_MCNP_partial_zaid(std::atoi(sv.substr(0, sv.find('.')).data()));
}

// ================================================================================================
// MCNP particle symbol
// -- Be aware that MCNP sometimes muddles the notation for particles relative to the PDG and
//    "alternate" standards, so the names used here won't necessary align with the names in the
//    MCNP manual.

const char invalid_mcnp_particle_symbol = ' ';

inline char to_MCNP_particle_symbol(EntityTag tag)
{
    if (tag.is_nuclide()) {
        // This format doens't support elementals
        if (tag.is_elemental()) {
            return invalid_mcnp_particle_symbol;
        }
        // This format doesn't support excited states
        // -- EntityTag fixes the Am-242g and Am-242m1 "swap", so ignore that special case here
        if (!tag.is_ground()) {
            return invalid_mcnp_particle_symbol;
        }
        const auto Z = tag.get_atomic_number();
        const auto A = tag.get_atomic_mass_number();
        if /*proton*/ ((Z == 1) && (A == 1)) {
            return 'H';
        } else if /*deuteron*/ ((Z == 1) && (A == 2)) {
            return 'D';
        } else if /*triton*/ ((Z == 1) && (A == 3)) {
            return 'T';
        } else if /*helion*/ ((Z == 2) && (A == 3)) {
            return 'S';
        } else if /*alpha*/ ((Z == 2) && (A == 4)) {
            return 'A';
        } else /*heavy ions*/ {
            return '#';
        }
    } else if (tag.is_particle()) {
        switch (tag.get_particle_index()) {
        case names::neutron: return 'N'; break;
        case names::photon: return 'P'; break;
        case names::electron: return 'E'; break;
        case names::muon: return '|'; break;
        case names::antineutron: return 'Q'; break;
        case names::electron_neutrino: return 'U'; break;
        case names::muon_neutrino: return 'V'; break;
        case names::positron: return 'F'; break;
        case names::proton: return 'H'; break;
        case names::neutral_lambda_baryon: return 'L'; break;
        case names::positive_sigma_baryon: return '+'; break;
        case names::negative_sigma_baryon: return '-'; break;
        case names::neutral_xi_baryon: return 'X'; break;
        case names::negative_xi_baryon: return 'Y'; break;
        case names::negative_omega_baryon: return 'O'; break;
        case names::antimuon: return '!'; break;
        case names::electron_antineutrino: return '<'; break;
        case names::muon_antineutrino: return '>'; break;
        case names::antiproton: return 'G'; break;
        case names::positive_pion: return '/'; break;
        case names::neutral_pion: return 'Z'; break;
        case names::positive_kaon: return 'K'; break;
        case names::short_kaon: return '%'; break;
        case names::long_kaon: return '^'; break;
        case names::neutral_lambda_antibaryon: return 'B'; break;
        case names::negative_sigma_antibaryon: return '_'; break;
        case names::positive_sigma_antibaryon: return '~'; break;
        case names::neutral_xi_antibaryon: return 'C'; break;
        case names::positive_xi_antibaryon: return 'W'; break;
        case names::positive_omega_antibaryon: return '@'; break;
        case names::negative_pion: return '*'; break;
        case names::negative_kaon: return '?'; break;
        default: return invalid_mcnp_particle_symbol;
        }
    } else {
        return invalid_mcnp_particle_symbol;
    }
}

inline EntityTag from_MCNP_particle_symbol(const char c)
{
    // Given that this is the "particle symbol", we prefer a particle to a nuclide in the case
    // where the choice is ambiguous (proton vs H-1)
    switch (c) {
    case 'N': return EntityTag(names::neutron); break;
    case 'P': return EntityTag(names::photon); break;
    case 'E': return EntityTag(names::electron); break;
    case '|': return EntityTag(names::muon); break;
    case 'Q': return EntityTag(names::antineutron); break;
    case 'U': return EntityTag(names::electron_neutrino); break;
    case 'V': return EntityTag(names::muon_neutrino); break;
    case 'F': return EntityTag(names::positron); break;
    case 'H': return EntityTag(names::proton); break;
    case 'L': return EntityTag(names::neutral_lambda_baryon); break;
    case '+': return EntityTag(names::positive_sigma_baryon); break;
    case '-': return EntityTag(names::negative_sigma_baryon); break;
    case 'X': return EntityTag(names::neutral_xi_baryon); break;
    case 'Y': return EntityTag(names::negative_xi_baryon); break;
    case 'O': return EntityTag(names::negative_omega_baryon); break;
    case '!': return EntityTag(names::antimuon); break;
    case '<': return EntityTag(names::electron_antineutrino); break;
    case '>': return EntityTag(names::muon_antineutrino); break;
    case 'G': return EntityTag(names::antiproton); break;
    case '/': return EntityTag(names::positive_pion); break;
    case 'Z': return EntityTag(names::neutral_pion); break;
    case 'K': return EntityTag(names::positive_kaon); break;
    case '%': return EntityTag(names::short_kaon); break;
    case '^': return EntityTag(names::long_kaon); break;
    case 'B': return EntityTag(names::neutral_lambda_antibaryon); break;
    case '_': return EntityTag(names::negative_sigma_antibaryon); break;
    case '~': return EntityTag(names::positive_sigma_antibaryon); break;
    case 'C': return EntityTag(names::neutral_xi_antibaryon); break;
    case 'W': return EntityTag(names::positive_xi_antibaryon); break;
    case '@': return EntityTag(names::positive_omega_antibaryon); break;
    case 'D': return EntityTag(1, 2); break;
    case 'T': return EntityTag(1, 3); break;
    case 'S': return EntityTag(2, 3); break;
    case 'A': return EntityTag(2, 4); break;
    case '*': return EntityTag(names::negative_pion); break;
    case '?': return EntityTag(names::negative_kaon); break;
    case '#': return EntityTag(EntityTag::unknown); break; // # can be _any_ heavy ion, so unknown
    default: return EntityTag(EntityTag::unknown);
    }
}

// ================================================================================================

} // end namespace nautilus::entity_tag

#endif // $ifndef NAUTILUS_FORMAT_MCNP_HPP
