#ifndef NAUTILUS_FORMAT_NDI_HPP
#define NAUTILUS_FORMAT_NDI_HPP

// TODO:
// -- NDI ZAID format
// -- NDI as-yet-unnamed format (SZZAAA.NNN, used by some host codes)
// -- NDI SZA format
// -- NDI short string

#include "nautilus/tag/pantag.hpp"

namespace nautilus::tag {

// ================================================================================================

namespace detail {

// TODO: What if someone passes the library name as a std::string (which doesn't work on GPUs until
//       at least C++20)?

struct NoLibrary {};

PORTABLE_FUNCTION constexpr inline bool standard_am242(NoLibrary) { return true; }
PORTABLE_FUNCTION constexpr inline bool standard_am242(const std::string_view & sv)
{
    if (sv == "mendf70x") {
        return false;
    } else if (sv == "mtmg01") {
        return false;
    } else if (sv == "mtmg01ex") {
        return false;
    } else if (sv == "701nm") {
        return false;
    } else {
        if (std::isdigit(sv[0]) && std::isdigit(sv[1]) && std::isdigit(sv[2]) && (sv[3] == 'n') &&
            (sv[4] == 'm')) {
            // C++17 introduces std::from_chars, but it's not constexpr until C++23
            std::array<char, 4> s = {sv[0], sv[1], sv[2], '\0'};
            const auto num = std::stoi(s.data());
            if ((num >= 121) && (num <= 135)) {
                return false;
            }
        }
        return true;
    }
}
// TODO: Thanks to floating-point truncation error, this is really unreliable.  I'm not convinced
//       we should provide it given that it's so easy to break, but instead require users to parse
//       their strings to get an integer.  But check with Edward, as he may have requested this
//       feature?
PORTABLE_FUNCTION constexpr inline bool standard_am242(const double d)
{
    if (d == 0.701) {
        return false;
    } else if ((d >= 0.121) && (d <= 0.135)) {
        return false;
    } else {
        return true;
    }
}
PORTABLE_FUNCTION constexpr inline bool standard_am242(const int n)
{
    if (n == 701) {
        return false;
    } else if ((n >= 121) && (n <= 135)) {
        return false;
    } else {
        return true;
    }
}

PORTABLE_FUNCTION constexpr inline bool standard_am244(NoLibrary) { return true; }
PORTABLE_FUNCTION constexpr inline bool standard_am244(const std::string_view & sv)
{
    if (sv == "endf7act") {
        return false;
    } else if (sv == "700nm") {
        return false;
    } else {
        return true;
    }
}
// TODO: Thanks to floating-point truncation error, this is really unreliable.  I'm not convinced
//       we should provide it given that it's so easy to break, but instead require users to parse
//       their strings to get an integer.  But check with Edward, as he may have requested this
//       feature?
PORTABLE_FUNCTION constexpr inline bool standard_am244(const double d)
{
    if (d == 0.700) {
        return false;
    } else {
        return true;
    }
}
PORTABLE_FUNCTION constexpr inline bool standard_am244(const int n)
{
    if (n == 700) {
        return false;
    } else {
        return true;
    }
}

PORTABLE_FUNCTION constexpr inline int suffix_integer(const int n) { return n; }
PORTABLE_FUNCTION constexpr inline int suffix_integer(const double d)
{
    return int(std::round(d * 1000));
}

} // end namespace detail

// ================================================================================================
// TODO: for SZA: int vs int32 vs int64?  signed vs unsigned?  templated for flexibility?

template <typename T>
PORTABLE_FUNCTION constexpr inline int to_NDI_SZA(const Pantag tag, T && library)
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
                    if (detail::standard_am242(std::forward<T>(library)) {
                        return 1095242;
                    } else {
                        return 95042
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

PORTABLE_FUNCTION constexpr inline auto to_NDI_SZA(const Pantag tag)
{
    // If the user doesn't provide a library, then just assume we use the most-standard NDI format
    return to_NDI_SZA(tag, NoLibrary())
}

PORTABLE_FUNCTION constexpr inline Pantag from_NDI_SZA(const int sza)
{
    switch (sza) {
    case (0): return Pantag(nautilus::tag::names::photon); break;
    case (1): return Pantag(nautilus::tag::names::neutron); break;
    case (1001): return Pantag(nautilus::tag::names::proton); break;
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
            case (1095242): return Pantag(95, 244, Pantag::Index::metastable, 1); break;
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

// TODO: Use SZA to build up to as-yet-unnamed format
// TODO: Bear in mind that we can't infer the suffix from the library name (some libraries have
//       multiple suffixes), but we can convert between strings (660nm), floats (0.660) and
//       integers (660).

// ================================================================================================

// TODO: Use as-yet-unnamed format to build up to ZAID
// TODO: Bear in mind that we can't infer the suffix from the library name, but we can convert
//       between strings (660nm), floats (0.660) and integers (660).  We can also (based on the
//       currently-available libraries) infer the full suffix from a partial suffix: they're all
//       three digits followed by "nm".  Will that continue to be true in the future, or is this a
//       feature we'll have to strip out if NDI starts using different characters in the suffix?

// ================================================================================================

} // end namespace nautilus::tag

#endif // $ifndef NAUTILUS_FORMAT_NDI_HPP
