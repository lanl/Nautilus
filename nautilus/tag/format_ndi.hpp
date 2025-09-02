#ifndef NAUTILUS_FORMAT_NDI_HPP
#define NAUTILUS_FORMAT_NDI_HPP

// TODO: Should a proton also map to 1001?
// TODO: What should happen if a Pantag doesn't map to the NDI formats?
//       -- Excited states: drop to ground state (lossy) or error?
//       -- Particles: default to 0 (lossy) or error?
//       -- User: ???
//       -- Unknown: ???
//       Add tests if an error isn't thrown

#include <algorithm>
#include <cmath>

#include "nautilus/tag/pantag.hpp"
#include "nautilus/tag/tokenize.hpp"

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

} // end namespace detail

// ================================================================================================
// NDI SZA

template <typename T>
int to_NDI_SZA(const Pantag tag, T && library)
{
    if (tag.is_particle()) {
        switch (tag.get_particle_index()) {
        case (nautilus::tag::names::photon): return 0; break;
        case (nautilus::tag::names::neutron): return 1; break;
        case (nautilus::tag::names::proton): return 1001; break;
        default:
            assert(false);
            return 0;
            break;
        }
        // Certain particles can also be encoded as ZAIDs: photon (0), neutron (1), proton (1001)
    } else {
        assert(tag.has_metastable_index());
        const auto Z = tag.get_atomic_number();
        const auto A = tag.get_atomic_mass_number();
        const auto S = tag.get_metastable_index();
        // some americium nuclides are messed up in NDI for historical reasons
        if (Z == 95) {
            if (A == 242) {
                // Am-242 swaps the S values for the ground state and the first metastable state
                if (S == 0) {
                    // Am-242g has two possible SZA values depending on the library
                    if (detail::standard_am242(std::forward<T>(library))) {
                        return 1095242;
                    } else {
                        return 95042;
                    }
                } else if (S == 1) {
                    return Z * 1000 + A;
                }
            } else if (A == 244) {
                // Am-244m1 has two possible SZA values depending on the library
                if (S == 1) {
                    if (detail::standard_am244(std::forward<T>(library))) {
                        return 1095244;
                    } else {
                        return 95044;
                    }
                }
            }
        }
        // standard cases fall through to here
        return (S * 1000 + Z) * 1000 + A;
    }
}

inline auto to_NDI_SZA(const Pantag tag)
{
    // If the user doesn't provide a library, then just assume we use the most-standard NDI format
    return to_NDI_SZA(tag, detail::NoLibrary());
}

inline Pantag from_NDI_SZA(const int sza)
{
    // TODO: Should 1001 -> proton or H-1?
    //    -- Are all NDI formats lossy?  Pantag(proton) --> 1001 --> Pantag(1,1)?
    switch (sza) {
    case (0): return Pantag(nautilus::tag::names::photon); break;
    case (1): return Pantag(nautilus::tag::names::neutron); break;
    default:
        auto remainder = sza;
        const auto A = remainder % 1000;
        remainder /= 1000;
        const auto Z = remainder % 1000;
        // special cases: some americium nuclides are messed up for historical reasons
        // We don't need to know the library, because the "special" values are unique across all
        // libraries.  If we see a special value we don't have to check the library name to know
        // how to interpret it, but can just parse it.
        if (Z == 95) {
            switch (sza) {
            // Am-242g (encoded as Am-242m1 in NDI, plus two different encoding strategies)
            case (95042): [[fallthrough]];
            case (1095242): return Pantag(95, 242); break;
            // Am-242m1 (encoded as Am-242g in NDI)
            case (95242): return Pantag(95, 242, Pantag::Index::metastable, 1); break;
            // Am-244m1 (two different encoding strategies)
            case (95044): [[fallthrough]];
            case (1095244): return Pantag(95, 244, Pantag::Index::metastable, 1); break;
            }
        }
        // standard cases will fall through to here
        if (A == 0) {
            return Pantag(Z, Pantag::elemental);
        } else {
            remainder /= 1000;
            const auto S = remainder;
            return Pantag(Z, A, Pantag::Index::metastable, S);
        }
    }
}

// ================================================================================================
// NDI FPID
// TODO: Still not sure about the name, but it's better than "the as-yet-unnamed format"
// TODO: Document that this is not recommended, but is provided to support people already using it.

template <typename T>
double to_NDI_FPID(Pantag tag, T && library)
{
    const auto SZA = to_NDI_SZA(tag, std::forward<T>(library));
    const double suffix = detail::table_suffix_decimal(std::forward<T>(library));
    return static_cast<double>(SZA) + suffix;
}

// Throw away the suffix (after the decimal) and coerce into an integer
// TODO: Pass the table suffix
inline Pantag from_NDI_FPID(const double fpid) { return from_NDI_SZA(static_cast<int>(fpid)); }

// ================================================================================================
// NDI zaid

template <typename T>
std::string to_NDI_zaid(Pantag tag, const T & library)
{
    assert(detail::match_table_suffix(library));
    std::string zaid = std::to_string(to_NDI_SZA(tag, library));
    zaid.append(".");
    zaid.append(detail::to_suffix_string(library));
    return zaid;
}

inline Pantag from_NDI_zaid(const std::string_view sv)
{
    // TODO: Pass the table suffix
    const auto pos = sv.find('.');
    return from_NDI_SZA(std::atoi(sv.substr(0, pos).data()));
}

// ================================================================================================
// NDI short string

inline std::string to_NDI_short_string(Pantag tag)
{
    if (tag.is_nuclide()) {
        assert(!tag.is_elemental());
        const auto Z = tag.get_atomic_number();
        const auto A = tag.get_atomic_mass_number();
        if ((Z == 95) && (A == 242)) {
            // Am-242g and Am-242m1 are reversed in NDI.  Since the short string doesn't deal with
            // excited states, that means we only handle Am-242m1
            assert(tag.has_metastable_index());
            assert(tag.get_metastable_index() == 1);
        } else {
            assert(tag.is_ground());
        }
        assert(tag.is_ground() || ((Z == 95) && (A == 242)));
        // handle special cases
        if (Z == 1) {
            if (A == 1) {
                return "p";
            } else if (A == 2) {
                return "d";
            } else if (A == 3) {
                return "t";
            }
        } else if (Z == 2) {
            if (A == 4) {
                return "a";
            }
        }
        // fallthrough to "standard" case
        // -- By ignoring the excited state:
        //    (a) we handle Am-242m1 (which maps to "am242")
        //    (b) in non-debug mode, anything with the wrong metastable state will automatically
        //        map to the default metastable state (ground state except for Am-242)
        std::string result(names::Nuclides::get_symbol(Z));
        result[0] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[0])));
        result.append(std::to_string(A));
        return result;
    } else {
        // Be aware that this format can be "lossy": (TODO, add to documentation)
        //    from_NDI_short_string(to_NDI_short_string(Pantag(proton))) --> Pantag(1,1)
        assert(tag.is_particle());
        const auto pidx = tag.get_particle_index();
        switch (pidx) {
        case (nautilus::tag::names::photon): return "g"; break;
        case (nautilus::tag::names::neutron): return "n"; break;
        case (nautilus::tag::names::proton): return "p"; break;
        default: assert(false); return "?";
        }
    }
}

inline Pantag from_NDI_short_string(const std::string_view sv)
{
    if ((sv == "g") || (sv == "g0")) {
        return Pantag(nautilus::tag::names::photon);
    } else if (sv == "n") {
        return Pantag(nautilus::tag::names::neutron);
    } else if (sv == "p") {
        return Pantag(1, 1);
    } else if (sv == "d") {
        return Pantag(1, 2);
    } else if (sv == "t") {
        return Pantag(1, 3);
    } else if (sv == "a") {
        return Pantag(2, 4);
    } else if (sv == "am242") {
        // Am-242g and Am-242m1 are swapped in NDI
        return Pantag(95, 242, Pantag::Index::metastable, 1);
    }
    const auto tokens = tokenize_nuclide(sv);
    // NDI short string does not have a hyphen between the symbol and the atomic mass number
    auto symbol = tokens[0];
    symbol[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(symbol[0])));
    const auto Z = names::Nuclides::find_index(symbol);
    if (Z == names::Nuclides::not_found) {
        return Pantag(Pantag::unknown);
    }
    // NDI short string does not permit elementals
    if (tokens[1].size() == 0) {
        return Pantag(Pantag::unknown);
    }
    assert(tokens[1][0] != '0'); // Would lead to bad parsing in std::stoi
    const auto A = std::stoi(tokens[1]);
    // NDI short string does not permit "g" suffix or excited states (except Am-242m1, see above)
    if (tokens[2].size() > 0) {
        return Pantag(Pantag::unknown);
    }
    return Pantag(Z, A);
}

// ================================================================================================

} // end namespace nautilus::tag

#endif // $ifndef NAUTILUS_FORMAT_NDI_HPP
