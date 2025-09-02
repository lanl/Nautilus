#ifndef NAUTILUS_FORMAT_NDI_HPP
#define NAUTILUS_FORMAT_NDI_HPP

// TODO: What should happen if a Pantag doesn't map to the IC chemsym format?

#include <algorithm>
#include <cmath>

#include "nautilus/tag/pantag.hpp"

namespace nautilus::tag {

// ================================================================================================

namespace detail {

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

//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// when converting to NDI, indicates we don't know the library so the default should be used
struct NoLibrary {};

//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline bool standard_am242(NoLibrary) { return true; }
inline bool standard_am242(const std::string_view sv)
{
    if (sv == "mendf70x") {
        return false;
    } else if (sv == "mtmg01") {
        return false;
    } else if (sv == "mtmg01ex") {
        return false;
    } else if ((sv == "701nm") || (sv == "701")) {
        return false;
    } else if (match_table_suffix(sv)) {
        const auto num = table_suffix_integer(sv);
        if ((num >= 121) && (num <= 135)) {
            return false;
        }
    }
    return true;
}
inline bool standard_am242(const int n)
{
    if (n == 701) {
        return false;
    } else if ((n >= 121) && (n <= 135)) {
        return false;
    } else {
        return true;
    }
}
// TODO: Thanks to floating-point truncation error, this is really unreliable.  I'm not convinced
//       we should provide it given that it's so easy to break, but instead require users to parse
//       their strings to get an integer.  But check with Edward, as he may have requested this
//       feature?
inline bool standard_am242(const double d) { return standard_am242(int(std::round(d * 1000))); }

//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline bool standard_am244(NoLibrary) { return true; }
inline bool standard_am244(const std::string_view sv)
{
    if (sv == "endf7act") {
        return false;
    } else if ((sv == "700nm") || (sv == "700")) {
        return false;
    } else {
        return true;
    }
}
inline bool standard_am244(const int n)
{
    if (n == 700) {
        return false;
    } else {
        return true;
    }
}
// TODO: Thanks to floating-point truncation error, this is really unreliable.  I'm not convinced
//       we should provide it given that it's so easy to break, but instead require users to parse
//       their strings to get an integer.  But check with Edward, as he may have requested this
//       feature?
inline bool standard_am244(const double d) { return standard_am244(int(std::round(d * 1000))); }

//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

// TODO: Below is untested.  Above hasn't been modified at all yet, so needs to be reviewed.

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
            const auto S = tag.get_metastable_index();
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

// Nuclides:
// -- Default: atomic symbol (lowercase), no hyphen, atomic mass number
//    -- Can have metastable state (but never 'm' without a number), 'g' is optional (not default)
//    -- Can also append table identifier number as ".###", accept on input but don't generate
// -- Elementals are the atomic symbol (lowercase) and nothing else
// -- Special cases
//    -- Am-242g can accept "am42", "am042", and "am242m1" on input; prefer "am242m1"
//    -- Am-242m1 can accept "am242", "am242g"; prefer "am242"
//    -- lawrencium is normally "Lr", but in this format it is "lw" (should lr be okay on input?)
// Particles:
// -- photons are "g" or "g0"; prefer "g0"
// -- neutrons are "nt1"
// -- protons are never generates, but should we accept "p" on input?
// -- no other particles

inline Pantag from_IC_chemsym(const std::string_view sv0)
{
    // TODO: Add a default of "unknown" if any step of parsing fails
    const std::string sv(sv0.begin(), sv0.begin + sv0.find('.'));
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
    const std::string numbers = "0123456789";
    const std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const auto it1 = std::find_first_of(sv.begin(), sv.end(), numbers.begin(), numbers.end());
    auto symbol = detail::to_uppercase_symbol(std::string(sv.begin(), it1));
    if (symbol == "lw") {
        symbol = "lr";
    }
    const auto Z = names::Nuclides::find_index(symbol);
    const auto it2 = std::find_first_of(it1, sv.end(), letters.begin(), letters.end());
    const auto A = std::atoi(std::string(it1, it2).data());
    const auto tail = std::string(it2, sv.end());
    int S = 0;
    switch (tail[0]) {
    case '\0': [[fallthrough]];
    case 'g': S = 0; break;
    case 'm': tail.erase(0, 1); S = std::stoi(tail); break;
    default: assert(false); 
    }
    return Pantag(Z, A, Pantag::Index::metastable, S);
}

// ================================================================================================

} // end namespace nautilus::tag

#endif // $ifndef NAUTILUS_FORMAT_NDI_HPP
