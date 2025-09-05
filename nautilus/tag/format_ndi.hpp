#ifndef NAUTILUS_FORMAT_NDI_HPP
#define NAUTILUS_FORMAT_NDI_HPP

// TODO: What types to use?
//    -- Currently I have SZA as int.  I should specify the precision (int32_t or similar) and
//       decide if it should be signed or unsigned.  If there's not a clear "best" answer, consider
//       templating on the SZA type.
//    -- Currently I have FPID as double.  I should check if it should be double or float.  If
//       there's not a clear "best" answer, consider templating on the SZA type.

// TODO: I currently have a mix of things with and without PORTABLE_FUNCTION.  Decide if SZA and/or
//       FPID should be available on the GPU.  I know zaid should not be available on the GPU
//       because it involves strings.

// TODO: In the documentation, explain that NDI formats are lossy because of the conversion
//       proton -> (NDI format) -> H-1

// TODO: In the documentation, explain that floating-point formats are not recommended, but are
//       only supported for customers already using such formats.  This includes specifying the
//       library by a floating-point suffix (e.g., 0.654) and the entire FPID format.

#include <algorithm>
#include <cmath>
#include <limits>

#include "nautilus/tag/pantag.hpp"
#include "nautilus/tag/string_processing.hpp"

namespace nautilus::tag {

// ================================================================================================

namespace detail {

// TODO: These are functions instead of constexpr values in case we need to template on the type
PORTABLE_FUNCTION inline constexpr int invalid_SZA()
{
    // TODO: This value won't work if SZA type is unsigned.
    return -1;
}
PORTABLE_FUNCTION inline constexpr double invalid_FPID()
{
    return std::numeric_limits<double>::max();
}
const std::string invalid_zaid = "unknown";
const std::string invalid_short_string = "unknown";

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
inline bool standard_am244(const double d) { return standard_am244(int(std::round(d * 1000))); }

} // end namespace detail

// ================================================================================================
// NDI SZA

template <typename T>
int to_NDI_SZA(const EntityTag tag, T && library)
{
    if (tag.is_particle()) {
        switch (tag.get_particle_index()) {
        case nautilus::tag::names::photon: return 0; break;
        case nautilus::tag::names::neutron: return 1; break;
        case nautilus::tag::names::proton: return 1001; break;
        default: return detail::invalid_SZA(); break;
        }
    } else if (tag.is_nuclide()) {
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
                    return 95242;
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
    } else {
        return detail::invalid_SZA();
    }
}

inline auto to_NDI_SZA(const EntityTag tag)
{
    // If the user doesn't provide a library, then just assume we use the most-standard NDI format
    return to_NDI_SZA(tag, detail::NoLibrary());
}

inline EntityTag from_NDI_SZA(const int sza)
{
    // Special cases
    switch (sza) {
    // particles (ignore proton, because H-1 is preferred when working with NDI)
    case 0: return EntityTag(nautilus::tag::names::photon); break;
    case 1: return EntityTag(nautilus::tag::names::neutron); break;
    // Am-242g (encoded as Am-242m1 in NDI, plus two different encoding strategies)
    case 95042: [[fallthrough]];
    case 1095242: return EntityTag(95, 242); break;
    // Am-242m1 (encoded as Am-242g in NDI)
    case 95242: return EntityTag(95, 242, 1); break;
    // Am-244m1 (two different encoding strategies)
    case 95044: [[fallthrough]];
    case 1095244: return EntityTag(95, 244, 1); break;
    }
    // Parse as a nuclide
    auto remainder = sza;
    const auto A = remainder % 1000;
    remainder /= 1000;
    const auto Z = remainder % 1000;
    if (Z == 0) {
        return EntityTag(EntityTag::unknown);
    }
    if (A == 0) { // NDI encodes elementals by setting A to zero
        return EntityTag(Z, EntityTag::elemental);
    } else {
        return EntityTag(Z, A, remainder / 1000);
    }
}

// ================================================================================================
// NDI FPID
// TODO: Still not sure about the name, but it's better than "the as-yet-unnamed format"

template <typename T>
double to_NDI_FPID(EntityTag tag, T && library)
{
    // TODO: check for invalid_SZA and translate to invalid_FPID
    const auto SZA = to_NDI_SZA(tag, std::forward<T>(library));
    const double suffix = detail::table_suffix_decimal(std::forward<T>(library));
    return static_cast<double>(SZA) + suffix;
}

// Throw away the suffix (after the decimal) and coerce into an integer
inline EntityTag from_NDI_FPID(const double fpid)
{
    // TODO: check for invalid_FPID
    // Throw away the suffix, because going _from_ NDI doesn't need to know the library
    return from_NDI_SZA(static_cast<int>(fpid));
}

// ================================================================================================
// NDI zaid

template <typename T>
std::string to_NDI_zaid(EntityTag tag, const T & library)
{
    // TODO: check for invalid_SZA and translate to invalid_zaid
    assert(detail::match_table_suffix(library));
    std::string zaid = std::to_string(to_NDI_SZA(tag, library));
    zaid.append(".");
    zaid.append(detail::to_suffix_string(library));
    return zaid;
}

inline EntityTag from_NDI_zaid(const std::string_view sv)
{
    // TODO: check for invalid_zaid
    return from_NDI_SZA(std::atoi(sv.substr(0, sv.find('.')).data()));
}

// ================================================================================================
// NDI short string

inline std::string to_NDI_short_string(EntityTag tag)
{
    if (tag.is_nuclide()) {
        const auto Z = tag.get_atomic_number();
        const auto A = tag.get_atomic_mass_number();
        // Elementals not supported by short string
        assert(!tag.is_elemental());
        // Short string doesn't support excited states
        if ((Z == 95) && (A == 242)) {
            // Am-242g and Am-242m1 are reversed in NDI, so Am-242m1 is allowed but Am-242g is not
            if (tag.get_metastable_index() != 1) {
                return detail::invalid_short_string;
            }
        } else {
            if (!tag.is_ground()) {
                return detail::invalid_short_string;
            }
        }
        // handle special cases
        if (Z == 1) {
            switch (A) {
            case 1: return "p"; break;
            case 2: return "d"; break;
            case 3: return "t"; break;
            }
        } else if ((Z == 2) && (A == 4)) {
            return "a";
        }
        // fallthrough to "standard" case
        if ((Z == 0) || (Z > names::Nuclides::count)) {
            return detail::invalid_short_string;
        }
        std::string result(names::Nuclides::get_symbol(Z));
        result[0] = to_lower(result[0]);
        result.append(std::to_string(A));
        return result;
    } else if (tag.is_particle()) {
        switch (tag.get_particle_index()) {
        case nautilus::tag::names::photon: return "g"; break;
        case nautilus::tag::names::neutron: return "n"; break;
        case nautilus::tag::names::proton: return "p"; break;
        default: return detail::invalid_short_string;
        }
    } else {
        return detail::invalid_short_string;
    }
}

inline EntityTag from_NDI_short_string(const std::string_view sv)
{
    // TODO: check for invalid_short_string
    // Particles and special cases
    if ((sv == "g") || (sv == "g0")) {
        return EntityTag(nautilus::tag::names::photon);
    } else if (sv == "n") {
        return EntityTag(nautilus::tag::names::neutron);
    } else if (sv == "p") {
        return EntityTag(1, 1);
    } else if (sv == "d") {
        return EntityTag(1, 2);
    } else if (sv == "t") {
        return EntityTag(1, 3);
    } else if (sv == "a") {
        return EntityTag(2, 4);
    } else if (sv == "am242") {
        // Am-242g and Am-242m1 are swapped in NDI
        return EntityTag(95, 242, 1);
    }
    const auto tokens = tokenize_nuclide(sv);
    // NDI short string does not have a hyphen between the symbol and the atomic mass number
    auto symbol = tokens[0];
    symbol[0] = to_upper(symbol[0]);
    const auto Z = names::Nuclides::find_index(symbol);
    if (Z == names::Nuclides::not_found) {
        return EntityTag(EntityTag::unknown);
    }
    // NDI short string does not permit elementals
    if (tokens[1].size() == 0) {
        return EntityTag(EntityTag::unknown);
    }
    assert(tokens[1][0] != '0'); // Would lead to bad parsing in std::stoi
    const auto A = std::stoi(tokens[1]);
    // NDI short string does not permit "g" suffix or excited states (except Am-242m1, see above)
    if (tokens[2].size() > 0) {
        return EntityTag(EntityTag::unknown);
    }
    return EntityTag(Z, A);
}

// ================================================================================================

} // end namespace nautilus::tag

#endif // $ifndef NAUTILUS_FORMAT_NDI_HPP
