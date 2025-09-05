#ifndef NAUTILUS_FORMAT_MCNP_HPP
#define NAUTILUS_FORMAT_MCNP_HPP

// TODO: What types to use?
//    -- Currently I have partial zaid as int.  I should specify the precision (int32_t or similar)
//       and decide if it should be signed or unsigned.  If there's not a clear "best" answer,
//       consider templating on the SZA type.

// TODO: I currently have a mix of things with and without PORTABLE_FUNCTION.  Decide if partial
//       zaid and particle symbol should be available on the GPU.  I know full zaid should not be
//       available on the GPU because it involves strings.

// TODO: In the documentation, explain that MCNP formats are lossy:
//    -- Full and partial zaid will map proton and H-1 to the same value, and that value will then
//       map back to Nautilus as H-1
//    -- Particle symbol will map proton and H-1 to the same value, and that value will then map
//       back to Nautilus as proton
//    -- Check all these possible changes, because my implementation may have changed some details.

#include <algorithm>
#include <cmath>
#include <limits>

#include "nautilus/entity_tag/entity_tag.hpp"
#include "nautilus/entity_tag/string_processing.hpp"

namespace nautilus::entity_tag {

// TODO: Move this discussion to the documentation
// See https://mcnp.lanl.gov/pdf_files/TechReport_2017_LANL_LA-UR-17-29981_WernerArmstrongEtAl.pdf,
// Table 3-32, page 3-56, for details of the MCNP zaid:
// -- A full zaid is ZZZAAA.abx, where
//    -- ZZZ is the atomic number
//    -- AAA is one of
//       -- 000 for elementals
//       -- the atomic mass number for ground-state nuclides
//       -- the atomic mass number + 300 + (m x 100) for metastable states (m = metastable index)
//    -- ab is the alphanumeric library identifier
//    -- x is the class of data (see Table 11-1 on page 11-1 for classes and their x value)
//    -- See Note 2 below the table (page 3-58): some(?) particles can be encoded in this format as
//       well, because the note says that photons and electrons use AAA = 000.  However, I cannot
//       find any further information about this, so Nautilus will not support particles.
//    -- Also discussed on page 1-13 under the item "Nuclide Identification Number (ZAID)"
// -- A partial zaid is just the ZZZAAA part of the full zaid (drop .abx)
// My notes say that Am-242 has g and m1 reversed as in NDI, but they hope to fix that in a future
// format (string-based) that's in progress.

// N.B.: MNCP sometimes uses the term "helion" for a He-3 nucleus (similar to "alpha" for a He-4
// nucleus or "deutron" for a H-2 nucleus or "triton" for a H-3 nucleus).

// MCNP also has the particle symbol, shown in Table 2-2 (page 2-11) of the document referenced
// above.  This format includes H-1 (as a proton), H-2 (as a deutron), H-3 (as a triton), He-3 (as
// a helion), and He-4 (as an alpha particle), and has a catch-all for "heavy ions" (noted to be
// any ion that's not one of the five previously-mentioned light ions).

// ================================================================================================

namespace detail {

// TODO: These are functions instead of constexpr values in case we need to template on the type
PORTABLE_FUNCTION inline constexpr int invalid_partial_zaid()
{
    // TODO: This value won't work if SZA type is unsigned.
    return -1;
}
const std::string invalid_full_zaid = "unknown";
const char invalid_particle_symbol = ' ';

//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline bool match_table_suffix(const std::string_view sv)
{
    if ((sv.length() != 3) && (sv.length() != 5)) {
        return false;
    } else if (!std::isdigit(sv[0])) {
        return false;
    } else if (!std::isdigit(sv[1])) {
        return false;
    } else if (!std::isdigit(sv[2])) {
        return false;
    } else if ((sv.length() == 5) && (sv[3] != 'n')) {
        return false;
    } else if ((sv.length() == 5) && (sv[4] != 'm')) {
        return false;
    }
    return true;
}
inline int table_suffix_integer(const std::string_view sv)
{
    assert(match_table_suffix);
    // C++17 introduces std::from_chars, but it's not constexpr until C++23
    std::array<char, 4> s = {sv[0], sv[1], sv[2], '\0'};
    return std::stoi(s.data());
}
inline double table_suffix_decimal(const int n) { return 1.0e-3 * n; }
inline double table_suffix_decimal(const double d) { return d; }
inline double table_suffix_decimal(const std::string_view sv)
{
    const auto num = table_suffix_integer(sv);
    return 1.0e-3 * num;
}

//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline std::string to_suffix_string(const int n)
{
    assert(n >= 0);
    assert(n < 1000);
    const auto ns = std::to_string(n);
    return std::string(3 - ns.size(), '0') + ns + "nm";
}
inline std::string to_suffix_string(const double d) { return to_suffix_string(int(d * 1000)); }
inline std::string to_suffix_string(const std::string_view sv)
{
    assert(match_table_suffix(sv));
    std::string suffix(sv);
    if (sv.length() == 3) {
        suffix.append("nm");
    }
    return suffix;
}

} // end namespace detail

// ================================================================================================
// MCNP partial zaid

inline int to_MCNP_partial_zaid(const EntityTag tag)
{
    if (!tag.is_nuclide()) {
        // The MCNP zaid is a nuclide-focused format that (as far as I can tell at the moment) does
        // not include any particles.  For this reason, we don't translate a proton (particle) to
        // the MCNP partial zaid format (where it would become H-1).
        // -- TODO: Shift this note to the documentation.
        return detail::invalid_partial_zaid();
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
        if (m > 4) {
            return detail::invalid_partial_zaid(); // MCNP partial zaid does not support m > 4
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

std::string to_MCNP_full_zaid(EntityTag tag, const std::string_view library)
{
    if (tag.is_unknown()) {
        return detail::invalid_full_zaid;
    }
    assert(detail::match_table_suffix(library));
    std::string zaid = std::to_string(to_MCNP_partial_zaid(tag));
    zaid.append(".");
    zaid.append(library);
    return zaid;
}

inline EntityTag from_MCNP_full_zaid(const std::string_view sv)
{
    if (sv == detail::invalid_full_zaid) {
        return EntityTag(EntityTag::unknown);
    }
    return from_MCNP_partial_zaid(std::atoi(sv.substr(0, sv.find('.')).data()));
}

// ================================================================================================
// MCNP particle symbol
// -- Be aware that MCNP sometimes muddles the notation for particles relative to the PDG and
//    "alternate" standards, so the names used here won't necessary align with the names in the
//    MCNP manual.

inline char to_MCNP_particle_symbol(EntityTag tag)
{
    if (tag.is_nuclide()) {
        // This format doens't support elementals
        if (tag.is_elemental()) {
            return detail::invalid_particle_symbol;
        }
        // This format doesn't support excited states
        // -- EntityTag fixes the Am-242g and Am-242m1 "swap", so ignore that special case here
        if (!tag.is_ground()) {
            return detail::invalid_particle_symbol;
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
        default: return detail::invalid_particle_symbol;
        }
    } else {
        return detail::invalid_particle_symbol;
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
