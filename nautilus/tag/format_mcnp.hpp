#ifndef NAUTILUS_FORMAT_NDI_HPP
#define NAUTILUS_FORMAT_NDI_HPP

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

#include <algorithm>
#include <cmath>
#include <limits>

#include "nautilus/tag/pantag.hpp"
#include "nautilus/tag/string_processing.hpp"

namespace nautilus::tag {

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

// Three formats:
// -- partial zaid (related to but distinct from NDI SZA)
// -- full zaid (related to but distinct from NDI zaid)
// -- particle symbol (no corresponding format in NDI)

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

inline int to_MCNP_partial_zaid(const Pantag tag)
{
    if (!tag.is_nuclide()) {
        // TODO: Should proton translate to H-1?
        return detail::invalid_partial_zaid();
    } else {
        assert(tag.has_metastable_index());
        const auto Z = tag.get_atomic_number();
        if (tag.is_elemental()) {
            return detail::invalid_partial_zaid();
        }
        const auto A = tag.get_atomic_mass_number();
        auto m = tag.get_metastable_index();
        // some americium nuclides are messed up in NDI for historical reasons
        if ((Z == 95) && (A == 242)) {
            // Am-242 swaps the S values for the ground state and the first metastable state
            if (m == 0) {
                m == 1;
            } else if (m == 1) {
                m == 0;
            }
        }
        // assemble the value
        auto partial_zaid = Z * 1000 + A;
        if (m != 0) {
            partial_zaid += 300 + m * 100;
        }
        return partial_zaid;
    }
}

inline Pantag from_MCNP_partial_zaid(const int partial_zaid)
{
    // Split into Z and A
    auto A = partial_zaid % 1000;
    const auto Z = partial_zaid / 1000;
    if (Z == 0) {
        return Pantag(Pantag::unknown);
    }
    // Check for metastable states
    int m = 0;
    // -- The heaviest known nuclide is a little below 300, and if there's a metastable state then
    //    we know A will be greater than 400 (300 + m * 100 + A, with m > 0 and A > 0)
    if (A > 400) {
        // TODO: Need to test this with a large number of isotopes, trying m = 0, 1, ...
        // Approximate A = f(Z)
        // -- Based on fitting hundreds of isotopes (across elements 1 - 118) found on Wikipedia.
        // -- Accurate to within +/- 10 for all isotopes tested.  The rounding strategy below
        //    requires that it be strictly within +/- 50 for all isotopes.
        // -- Conveniently, the cubic fit always gives non-negative values for A and N = A - Z (not
        //    sure if that's required, but it's nice to have).
        auto f = [](const int Z) {
            const auto Z2 = Z * Z;
            const auto Z3 = Z * Z2;
            return -0.0000711325*Z3 + 0.0147356*Z2 + 1.75397*Z + 2.04063;
        };
        const int m = std::round(0.01 * (A - 300 - f(Z)));
        assert(m > 0);        // sanity check: if A > 400 then we must have an excited state
        A -= (300 + m * 100); // remove the metastable correction from the atomic mass number
        assert(A >= Z);       // sanity check: cannot have negative neutrons
    }
    // special cases: Am-242g and Am-242m1 are swapped
    if ((Z == 95) && (A == 242)) {
        if (m == 0) {
            m = 1;
        } else if (m == 1) {
            m = 0;
        }
    }
    // construct the Pantag
    if /* elemental */ (A == 0) {
        return Pantag(Z, Pantag::elemental);
    } else if /* ground state */ (m == 0) {
        return Pantag(Z, A);
    } else /* excited state */ {
        return Pantag(Z, A, Pantag::Index::metastable, m);
    }
}

// ================================================================================================
// MCNP full zaid

std::string to_MCNP_full_zaid(Pantag tag, const std::string_view library)
{
    // TODO: check for invalid_partial_zaid and transform to invalid_full_zaid
    assert(detail::match_table_suffix(library));
    std::string zaid = std::to_string(to_MCNP_partial_zaid(tag));
    zaid.append(".");
    zaid.append(library);
    return zaid;
}

inline Pantag from_MCNP_full_zaid(const std::string_view sv)
{
    // TODO: check for invalid_full_zaid
    return from_MCNP_partial_zaid(std::atoi(sv.substr(0, sv.find('.')).data()));
}

// ================================================================================================
// MCNP particle symbol
// -- Be aware that MCNP sometimes muddles the notation for particles relative to the PDG and
//    "alternate" standards, so the names used here won't necessary align with the names in the
//    MCNP manual.

inline char to_MCNP_particle_symbol(Pantag tag)
{
    if (tag.is_user() || tag.is_unknown()) {
        return detail::invalid_particle_symbol;
    } else if (tag.is_nuclide()) {
        // This format doens't use elementals
        if (tag.is_elemental()) {
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
    } else {
        assert(tag.is_particle());
        const auto pidx = tag.get_particle_index();
        switch (pidx) {
        case nautilus::tag::names::neutron: return 'N'; break;
        case nautilus::tag::names::photon: return 'P'; break;
        case nautilus::tag::names::electron: return 'E'; break;
        case nautilus::tag::names::muon: return '|'; break;
        case nautilus::tag::names::antineutron: return 'Q'; break;
        case nautilus::tag::names::electron_neutrino: return 'U'; break;
        case nautilus::tag::names::muon_neutrino: return 'V'; break;
        case nautilus::tag::names::positron: return 'F'; break;
        case nautilus::tag::names::proton: return 'H'; break;
        case nautilus::tag::names::neutral_lambda_baryon: return 'L'; break;
        case nautilus::tag::names::positive_sigma_baryon: return '+'; break;
        case nautilus::tag::names::negative_sigma_baryon: return '-'; break;
        case nautilus::tag::names::neutral_xi_baryon: return 'X'; break;
        case nautilus::tag::names::negative_xi_baryon: return 'Y'; break;
        case nautilus::tag::names::negative_omega_baryon: return 'O'; break;
        case nautilus::tag::names::antimuon: return '!'; break;
        case nautilus::tag::names::electron_antineutrino: return '<'; break;
        case nautilus::tag::names::muon_antineutrino: return '>'; break;
        case nautilus::tag::names::antiproton: return 'G'; break;
        case nautilus::tag::names::positive_pion: return '/'; break;
        case nautilus::tag::names::neutral_pion: return 'Z'; break;
        case nautilus::tag::names::positive_kaon: return 'K'; break;
        case nautilus::tag::names::short_kaon: return '%'; break;
        case nautilus::tag::names::long_kaon: return '^'; break;
        case nautilus::tag::names::neutral_lambda_antibaryon: return 'B'; break;
        case nautilus::tag::names::negative_sigma_antibaryon: return '_'; break;
        case nautilus::tag::names::positive_sigma_antibaryon: return '~'; break;
        case nautilus::tag::names::neutral_xi_antibaryon: return 'C'; break;
        case nautilus::tag::names::positive_xi_antibaryon: return 'W'; break;
        case nautilus::tag::names::positive_omega_antibaryon: return '@'; break;
        case nautilus::tag::names::negative_pion: return '*'; break;
        case nautilus::tag::names::negative_kaon: return '?'; break;
        default: return detail::invalid_particle_symbol;
        }
    }
}

inline Pantag from_MCNP_particle_symbol(const char c)
{
    // Given that this is the "particle symbol", we prefer a particle to a nuclide in the case
    // where the choice is ambiguous (proton vs H-1)
    switch (c) {
    case 'N': return Pantag(nautilus::tag::names::neutron); break;
    case 'P': return Pantag(nautilus::tag::names::photon); break;
    case 'E': return Pantag(nautilus::tag::names::electron); break;
    case '|': return Pantag(nautilus::tag::names::muon); break;
    case 'Q': return Pantag(nautilus::tag::names::antineutron); break;
    case 'U': return Pantag(nautilus::tag::names::electron_neutrino); break;
    case 'V': return Pantag(nautilus::tag::names::muon_neutrino); break;
    case 'F': return Pantag(nautilus::tag::names::positron); break;
    case 'H': return Pantag(nautilus::tag::names::proton); break;
    case 'L': return Pantag(nautilus::tag::names::neutral_lambda_baryon); break;
    case '+': return Pantag(nautilus::tag::names::positive_sigma_baryon); break;
    case '-': return Pantag(nautilus::tag::names::negative_sigma_baryon); break;
    case 'X': return Pantag(nautilus::tag::names::neutral_xi_baryon); break;
    case 'Y': return Pantag(nautilus::tag::names::negative_xi_baryon); break;
    case 'O': return Pantag(nautilus::tag::names::negative_omega_baryon); break;
    case '!': return Pantag(nautilus::tag::names::antimuon); break;
    case '<': return Pantag(nautilus::tag::names::electron_antineutrino); break;
    case '>': return Pantag(nautilus::tag::names::muon_antineutrino); break;
    case 'G': return Pantag(nautilus::tag::names::antiproton); break;
    case '/': return Pantag(nautilus::tag::names::positive_pion); break;
    case 'Z': return Pantag(nautilus::tag::names::neutral_pion); break;
    case 'K': return Pantag(nautilus::tag::names::positive_kaon); break;
    case '%': return Pantag(nautilus::tag::names::short_kaon); break;
    case '^': return Pantag(nautilus::tag::names::long_kaon); break;
    case 'B': return Pantag(nautilus::tag::names::neutral_lambda_antibaryon); break;
    case '_': return Pantag(nautilus::tag::names::negative_sigma_antibaryon); break;
    case '~': return Pantag(nautilus::tag::names::positive_sigma_antibaryon); break;
    case 'C': return Pantag(nautilus::tag::names::neutral_xi_antibaryon); break;
    case 'W': return Pantag(nautilus::tag::names::positive_xi_antibaryon); break;
    case '@': return Pantag(nautilus::tag::names::positive_omega_antibaryon); break;
    case 'D': return Pantag(1, 2); break;
    case 'T': return Pantag(1, 3); break;
    case 'S': return Pantag(2, 3); break;
    case 'A': return Pantag(2, 4); break;
    case '*': return Pantag(nautilus::tag::names::negative_pion); break;
    case '?': return Pantag(nautilus::tag::names::negative_kaon); break;
    case '#': return Pantag(Pantag::unknown); break; // # can be _any_ heavy ion, thus unknown
    default: return Pantag(Pantag::unknown);
    }
}

// ================================================================================================

} // end namespace nautilus::tag

#endif // $ifndef NAUTILUS_FORMAT_NDI_HPP
