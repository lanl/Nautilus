#ifndef NAUTILUS_FORMAT_NDI_HPP
#define NAUTILUS_FORMAT_NDI_HPP

// TODO: What should happen if a Pantag doesn't map to the IC chemsym format?

#include <algorithm>
#include <cmath>

#include "nautilus/tag/pantag.hpp"
#include "nautilus/tag/tokenize.hpp"

namespace nautilus::tag {

// ================================================================================================

namespace detail {

// specifically to modify the atomic symbol, not general-purpose routines
// TODO: This is duplicated in NDI and IC.  I suspect it should move to names.hpp instead.

inline std::string to_lowercase_symbol(std::string_view sv)
{
    std::string result;
    switch (sv[0]) {
    case ('A'): result.append(1, 'a'); break;
    case ('B'): result.append(1, 'b'); break;
    case ('C'): result.append(1, 'c'); break;
    case ('D'): result.append(1, 'd'); break;
    case ('E'): result.append(1, 'e'); break;
    case ('F'): result.append(1, 'f'); break;
    case ('G'): result.append(1, 'g'); break;
    case ('H'): result.append(1, 'h'); break;
    case ('I'): result.append(1, 'i'); break;
    case ('J'): result.append(1, 'j'); break;
    case ('K'): result.append(1, 'k'); break;
    case ('L'): result.append(1, 'l'); break;
    case ('M'): result.append(1, 'm'); break;
    case ('N'): result.append(1, 'n'); break;
    case ('O'): result.append(1, 'o'); break;
    case ('P'): result.append(1, 'p'); break;
    case ('Q'): result.append(1, 'q'); break;
    case ('R'): result.append(1, 'r'); break;
    case ('S'): result.append(1, 's'); break;
    case ('T'): result.append(1, 't'); break;
    case ('U'): result.append(1, 'u'); break;
    case ('V'): result.append(1, 'v'); break;
    case ('W'): result.append(1, 'w'); break;
    case ('X'): result.append(1, 'x'); break;
    case ('Y'): result.append(1, 'y'); break;
    case ('Z'): result.append(1, 'z'); break;
    default: assert(false);
    }
    if (sv.size() > 1) {
        result.append(1, sv[1]);
    }
    return result;
}
inline std::string to_uppercase_symbol(std::string_view sv)
{
    std::string result;
    switch (sv[0]) {
    case ('a'): result.append(1, 'A'); break;
    case ('b'): result.append(1, 'B'); break;
    case ('c'): result.append(1, 'C'); break;
    case ('d'): result.append(1, 'D'); break;
    case ('e'): result.append(1, 'E'); break;
    case ('f'): result.append(1, 'F'); break;
    case ('g'): result.append(1, 'G'); break;
    case ('h'): result.append(1, 'H'); break;
    case ('i'): result.append(1, 'I'); break;
    case ('j'): result.append(1, 'J'); break;
    case ('k'): result.append(1, 'K'); break;
    case ('l'): result.append(1, 'L'); break;
    case ('m'): result.append(1, 'M'); break;
    case ('n'): result.append(1, 'N'); break;
    case ('o'): result.append(1, 'O'); break;
    case ('p'): result.append(1, 'P'); break;
    case ('q'): result.append(1, 'Q'); break;
    case ('r'): result.append(1, 'R'); break;
    case ('s'): result.append(1, 'S'); break;
    case ('t'): result.append(1, 'T'); break;
    case ('u'): result.append(1, 'U'); break;
    case ('v'): result.append(1, 'V'); break;
    case ('w'): result.append(1, 'W'); break;
    case ('x'): result.append(1, 'X'); break;
    case ('y'): result.append(1, 'Y'); break;
    case ('z'): result.append(1, 'Z'); break;
    default: assert(false);
    }
    if (sv.size() > 1) {
        result.append(1, sv[1]);
    }
    return result;
}

} // end namespace detail

// ================================================================================================
// IC chemsym

inline std::string to_IC_chemsym(Pantag tag)
{
    if (tag.is_nuclide()) {
        const auto Z = tag.get_atomic_number();
        std::string result = detail::to_lowercase_symbol(names::Nuclides::get_symbol(Z));
        // lawrencium is uses a pre-standard symbol and was never updated after standardization
        if (result == "lr") {
            result = "lw";
        }
        if (tag.is_elemental()) {
            // elementals are just the atomic symbol
            return result;
        } else {
            // append the atomic mass number
            const auto A = tag.get_atomic_mass_number();
            result.append(std::to_string(A));
            // append the metastable state if not in the ground state
            // -- can't just check tag.is_ground() because Am-242g and Am-242m1 are swapped
            assert(tag.has_metastable_index());
            auto S = tag.get_metastable_index();
            if ((Z == 95) && (A == 242)) {
                if (S == 0) {
                    S = 1;
                } else if (S == 1) {
                    S = 0;
                }
            }
            if (S > 0) {
                result.append("m");
                result.append(std::to_string(S));
            }
            return result;
        }
    } else {
        assert(tag.is_particle());
        const auto pidx = tag.get_particle_index();
        switch (pidx) {
        case (nautilus::tag::names::photon): return "g0"; break;
        case (nautilus::tag::names::neutron): return "nt1"; break;
        default: assert(false); return "?";
        }
    }
}

inline Pantag from_IC_chemsym(const std::string_view sv0)
{
    const std::string sv(sv0.substr(0, sv0.find('.')));
    if ((sv == "g") || (sv == "g0")) {
        return Pantag(nautilus::tag::names::photon);
    } else if (sv == "nt1") {
        return Pantag(nautilus::tag::names::neutron);
    } else if ((sv == "am242") || (sv == "am242g")) {
        // Am-242g and Am-242m1 are swapped in NDI
        return Pantag(95, 242, Pantag::Index::metastable, 1);
    } else if ((sv == "am42") || (sv == "am042") || (sv == "am242m1")) {
        // Am-242g and Am-242m1 are swapped in NDI
        return Pantag(95, 242);
    }
    // If we fall through to here, we assume this is a nuclide
    const auto tokens = tokenize_nuclide(sv);
    // Get the atomic number
    auto symbol = detail::to_uppercase_symbol(tokens[0]);
    if (symbol == "lw") { // lawrencium is non-standard
        symbol = "lr";
    }
    const auto Z = names::Nuclides::find_index(symbol);
    if (Z == names::Nuclides::not_found) {
        return Pantag(Pantag::unknown);
    }
    // Get the atomic mass number
    if (tokens[1].size() == 0) {
        return Pantag(Z, Pantag::elemental);
    }
    const auto A = std::stoi(tokens[1]);
    // Get the excitation index
    const auto c = tokens[2][0];
    assert((c == 'g') || (c == 'm') || (c == '\0'));
    switch(c) {
    case 'g': [[fallthrough]];
    case '\0': return Pantag(Z, A); break;
    case 'm':
        return Pantag(Z, A, Pantag::Index::metastable, std::stoi(tokens[2].substr(1)));
        break;
    default: return Pantag(Pantag::unknown);
    }
}

// ================================================================================================

} // end namespace nautilus::tag

#endif // $ifndef NAUTILUS_FORMAT_NDI_HPP
