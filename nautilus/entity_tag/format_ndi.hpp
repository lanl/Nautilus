#ifndef NAUTILUS_FORMAT_NDI_HPP
#define NAUTILUS_FORMAT_NDI_HPP

#include <algorithm>
#include <charconv>
#include <cmath>
#include <limits>
#include <vector>

#include "nautilus/entity_tag/entity_tag.hpp"
#include "nautilus/util/string_processing.hpp"

namespace nautilus::entity_tag {

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
    int result = 0;
    std::from_chars(sv.begin(), sv.begin() + 3, result);
    return result;
}

inline double table_suffix_decimal(const int n) { return 1.0e-3 * n; }
inline double table_suffix_decimal(const double d) { return d; }
inline double table_suffix_decimal(const std::string_view sv)
{
    const auto num = table_suffix_integer(sv);
    return 1.0e-3 * num;
}

inline std::string table_suffix_string(const int n)
{
    assert(n >= 0);
    assert(n < 1000);
    const auto ns = std::to_string(n);
    return std::string(3 - ns.size(), '0') + ns + "nm";
}
inline std::string table_suffix_string(const double d)
{
    return table_suffix_string(static_cast<int>(d * 1000));
}
inline std::string table_suffix_string(const std::string_view sv)
{
    assert(match_table_suffix(sv));
    std::string suffix(sv);
    if (sv.length() == 3) {
        suffix.append("nm");
    }
    return suffix;
}

//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template <typename T, typename Container>
bool contains(const Container & c, const T & value)
{
    return std::find(c.begin(), c.end(), value) != c.end();
}
// when converting to NDI, indicates we don't know the library so the default should be used
struct NoLibrary {};
// Special cases for Am-242
struct Am242 {
    static inline bool is_standard(NoLibrary) { return true; }
    static inline bool is_standard(const int num)
    {
        // As of C++20 we can make std::vector constexpr, which means we don't have to rebuild the
        // list of table IDs every time this function is called.
        std::vector<int> table_ids;
        table_ids.push_back(701);
        for (int n = 121; n <= 135; ++n) {
            table_ids.push_back(n);
        }
        return !contains(table_ids, num);
    }
    static inline bool is_standard(const double d)
    {
        return is_standard(static_cast<int>(std::round(d * 1000)));
    }
    static inline bool is_standard(const std::string & str)
    {
        if (match_table_suffix(str)) {
            return is_standard(table_suffix_integer(str));
        }
        const std::vector<std::string> libraries{"mendf70x", "mtmg01", "mtmg01ex"};
        return !contains(libraries, str);
    }
};
// Special cases for Am-244
struct Am244 {
    static inline bool is_standard(NoLibrary) { return true; }
    static inline bool is_standard(const int num)
    {
        std::vector<int> table_ids{700};
        return !contains(table_ids, num);
    }
    static inline bool is_standard(const double d)
    {
        return is_standard(static_cast<int>(std::round(d * 1000)));
    }
    static inline bool is_standard(const std::string & str)
    {
        if (match_table_suffix(str)) {
            return is_standard(table_suffix_integer(str));
        }
        const std::vector<std::string> libraries{"endf7act"};
        return !contains(libraries, str);
    }
};

} // end namespace detail

// ================================================================================================
// NDI SZA

constexpr int invalid_ndi_sza = -1;

template <typename T>
int to_NDI_SZA(const EntityTag tag, T && library)
{
    if (tag.is_particle()) {
        switch (tag.get_particle_index()) {
        case names::photon: return 0; break;
        case names::neutron: return 1; break;
        case names::proton: return 1001; break;
        default: return invalid_ndi_sza; break;
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
                    if (detail::Am242::is_standard(std::forward<T>(library))) {
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
                    if (detail::Am244::is_standard(std::forward<T>(library))) {
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
        return invalid_ndi_sza;
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
    case 0: return EntityTag(names::photon); break;
    case 1: return EntityTag(names::neutron); break;
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

constexpr double invalid_ndi_fpid = std::numeric_limits<double>::max();

template <typename T>
double to_NDI_FPID(EntityTag tag, T && library)
{
    const auto SZA = to_NDI_SZA(tag, std::forward<T>(library));
    if (SZA == invalid_ndi_sza) {
        return invalid_ndi_fpid;
    }
    const double suffix = detail::table_suffix_decimal(std::forward<T>(library));
    return static_cast<double>(SZA) + suffix;
}

// Throw away the suffix (after the decimal) and coerce into an integer
inline EntityTag from_NDI_FPID(const double fpid)
{
    if (fpid == invalid_ndi_fpid) {
        return EntityTag(EntityTag::unknown);
    }
    // Throw away the suffix, because going _from_ NDI doesn't need to know the library
    return from_NDI_SZA(static_cast<int>(fpid));
}

// ================================================================================================
// NDI zaid

const std::string invalid_ndi_zaid = "unknown";

template <typename T>
std::string to_NDI_zaid(EntityTag tag, const T & library)
{
    assert(detail::match_table_suffix(library));
    const auto SZA = to_NDI_SZA(tag, library);
    if (SZA == invalid_ndi_sza) {
        return invalid_ndi_zaid;
    }
    std::string zaid = std::to_string(SZA);
    zaid.append(".");
    zaid.append(detail::table_suffix_string(library));
    return zaid;
}

inline EntityTag from_NDI_zaid(const std::string_view sv)
{
    if (sv == invalid_ndi_zaid) {
        return EntityTag(EntityTag::unknown);
    }
    return from_NDI_SZA(std::atoi(sv.substr(0, sv.find('.')).data()));
}

// ================================================================================================
// NDI short string

const std::string invalid_ndi_short_string = "?";

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
                return invalid_ndi_short_string;
            }
        } else {
            if (!tag.is_ground()) {
                return invalid_ndi_short_string;
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
            return invalid_ndi_short_string;
        }
        std::string result(names::Nuclides::get_symbol(Z));
        result[0] = to_lower(result[0]);
        result.append(std::to_string(A));
        return result;
    } else if (tag.is_particle()) {
        switch (tag.get_particle_index()) {
        case names::photon: return "g"; break;
        case names::neutron: return "n"; break;
        case names::proton: return "p"; break;
        default: return invalid_ndi_short_string;
        }
    } else {
        return invalid_ndi_short_string;
    }
}

inline EntityTag from_NDI_short_string(const std::string_view sv)
{
    // Unknown, particles, special cases
    if (sv == invalid_ndi_short_string) {
        return EntityTag(EntityTag::unknown);
    } else if ((sv == "g") || (sv == "g0")) {
        return EntityTag(names::photon);
    } else if (sv == "n") {
        return EntityTag(names::neutron);
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

} // end namespace nautilus::entity_tag

#endif // $ifndef NAUTILUS_FORMAT_NDI_HPP
