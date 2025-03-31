#ifndef NAUTILUS_FORMAT_STANDARD_HPP
#define NAUTILUS_FORMAT_STANDARD_HPP

#include "nautilus/names.hpp"
#include "nautilus/tag/pantag.hpp"

#include "ports-of-call/portability.hpp"

#include <array>
#include <cassert>
#include <string>
#include <string_view>

// Note: There are versions with and without the suffix "_portable" on the function name.  The
//       versions without the suffix are designed for ease-of-use and assume that the code is
//       running on a CPU.  For that reason, std::string is the output type as that would typically
//       be expected by C++ developers.  The portable versions are designed to run on GPUs, but
//       std::string is not constexpr and can't be used on GPUs as of this writing.  Instead, the
//       return type is std::array<char, len>.  Calling the data() method of the return value
//       provides a pointer to a null-terminated character array (i.e., a C-style string).

namespace nautilus::tag {

// ================================================================================================

// 123456789
// elemental

// longest element: 12 characters

// short nuclides can be up to 10 characters
// short particles can be up to 7 characters
// need to pad with one extra character for a null terminator
// round up to next power of 2
constexpr std::size_t SHORT_LEN = 16;

// long nuclides can be up to 20 characters
// long particles (PDG standard) can be up to 25 characters
// long particles ("alternate" standard) can be up to 41 characters
// need to pad with one extra character for a null terminator
// round up to next power of 2
constexpr std::size_t LONG_LEN = 64;

// ================================================================================================

namespace detail {

PORTABLE_FUNCTION constexpr char digit(int n) {
    switch(n) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    default: return '\0';
    }
}

PORTABLE_FUNCTION constexpr void append(char*& buf, char c) {
    *(buf++) = c;
}

PORTABLE_FUNCTION constexpr void append(char*& buf, std::string_view sv) {
    for (char c : sv) {
        append(buf, c);
    }
}

// std::reverse is constexpr in C++20
PORTABLE_FUNCTION constexpr void reverse(char* lo, char* hi) {
    --hi;
    for(; hi > lo; ++lo, --hi) {
        char tmp = *lo;
        *lo = *hi;
        *hi = tmp;
    }
}

PORTABLE_FUNCTION constexpr void append(char*& buf, uint32_t n) {
    if (n == 0) {
        append(buf, '0');
    } else {
        char* buf0 = buf;
        while (n > 0) {
            append(buf, digit(n % 10));
            n /= 10;
        }
        reverse(buf0, buf);
    }
}

PORTABLE_FUNCTION constexpr void append(char*& buf, uint64_t n) {
    if (n == 0) {
        append(buf, '0');
    } else {
        char* buf0 = buf;
        while (n > 0) {
            append(buf, digit(n % 10));
            n /= 10;
        }
        reverse(buf0, buf);
    }
}

template <std::size_t N>
PORTABLE_FUNCTION constexpr std::array<char, N> null_array()
{
    std::array<char, N> name;
    for (auto & c : name) {
        c = '\0';
    }
    return name;
}

} // namespace detail

// ================================================================================================

// longest IUPAC symbol: 10 characters
// -- 2 characters for element name
// -- 4 characters for atomic mass number ("-###")
// -- 4 characters for excitation/metastable suffix ("[em]###")
PORTABLE_FUNCTION constexpr std::array<char, SHORT_LEN> to_short_iupac_name_portable(
    const Pantag tag)
{
    assert(tag.is_nuclide() && tag.is_standard());
    auto name = detail::null_array<SHORT_LEN>();
    char* ptr = name.data();
    detail::append(ptr, names::Nuclides::get_symbol(tag.get_atomic_number()));
    if (!tag.is_elemental()) {
        detail::append(ptr, '-');
        detail::append(ptr, tag.get_atomic_mass_number());
        if (!tag.is_ground()) {
            detail::append(ptr, tag.has_metastable_index() ? 'm' : 'e');
            detail::append(ptr, tag.get_index()); // TODO: is get_index a good name?
        }
    }
    return name;
}
std::string to_short_iupac_name(const Pantag tag)
{
    return to_short_iupac_name_portable(tag).data();
}

// ================================================================================================

PORTABLE_FUNCTION constexpr std::array<char, LONG_LEN> to_long_iupac_name_portable(
    const Pantag tag, const names::Nuclides::Standard standard = names::Nuclides::Standard(0))
{
    assert(tag.is_nuclide() && tag.is_standard());
    auto name = detail::null_array<LONG_LEN>();
    char* ptr = name.data();
    if (tag.is_elemental()) {
        detail::append(ptr, "elemental ");
    }
    detail::append(ptr, names::Nuclides::get_name(tag.get_atomic_number(), standard));
    if (!tag.is_elemental()) {
        detail::append(ptr, '-');
        detail::append(ptr, tag.get_atomic_mass_number());
        if (!tag.is_ground()) {
            detail::append(ptr, tag.has_metastable_index() ? 'm' : 'e');
            detail::append(ptr, tag.get_index());
        }
    }
    return name;
}
std::string to_long_iupac_name(
    const Pantag tag, const names::Nuclides::Standard standard = names::Nuclides::Standard(0))
{
    return to_long_iupac_name_portable(tag, standard).data();
}

// ================================================================================================

PORTABLE_FUNCTION constexpr std::array<char, SHORT_LEN> to_short_pdg_name_portable(const Pantag tag)
{
    assert(tag.is_particle() && tag.is_standard());
    auto name = detail::null_array<SHORT_LEN>();
    char* ptr = name.data();
    detail::append(ptr, names::Particles::get_symbol(tag.get_atomic_number()));
    return name;
}
std::string to_short_pdg_name(const Pantag tag)
{
    return to_short_pdg_name_portable(tag).data();
}

// ================================================================================================

PORTABLE_FUNCTION constexpr std::array<char, LONG_LEN> to_long_pdg_name_portable(
    const Pantag tag, const names::Particles::Standard standard = names::Particles::Standard(0))
{
    assert(tag.is_particle() && tag.is_standard());
    auto name = detail::null_array<LONG_LEN>();
    char* ptr = name.data();
    detail::append(ptr, names::Particles::get_name(tag.get_particle_index(), standard));
    return name;
}
std::string to_long_pdg_name(const Pantag tag, const names::Particles::Standard standard)
{
    return to_long_pdg_name_portable(tag, standard).data();
}

// ================================================================================================

// For symbols:
// -- nuclides only have a single standard
// -- the symbols for the "alternate" particle convention cannot all be represented in Unicode, so
//    you always get the PDG symbol
std::array<char, SHORT_LEN> to_short_standard_name_portable(const Pantag tag)
{
    if (tag.is_nuclide())   return to_short_iupac_name_portable(tag);
    else                    return to_short_pdg_name_portable(tag);
}
std::string to_short_standard_name(const Pantag tag)
{
    if (tag.is_nuclide())   return to_short_iupac_name(tag);
    else                    return to_short_pdg_name(tag);
}

// ================================================================================================

// Variations of the same thing so that the user can specify a nuclide standard and/or a particle
// standard, and if they specify both then the order is irrelevant.
std::array<char, LONG_LEN> to_long_standard_name_portable(
    const Pantag tag,
    const names::Nuclides::Standard nuclide_standard,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0))
{
    if (tag.is_nuclide())   return to_long_iupac_name_portable(tag, nuclide_standard);
    else                    return to_long_pdg_name_portable(tag, particle_standard);
}
std::array<char, LONG_LEN> to_long_standard_name_portable(
    const Pantag tag,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0),
    const names::Nuclides::Standard nuclide_standard = names::Nuclides::Standard(0))
{
    return to_long_standard_name_portable(tag, nuclide_standard, particle_standard);
}

// ================================================================================================

// Variations of the same thing so that the user can specify a nuclide standard and/or a particle
// standard, and if they specify both then the order is irrelevant.
std::string to_long_standard_name(
    const Pantag tag,
    const names::Nuclides::Standard nuclide_standard,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0))
{
    if (tag.is_nuclide())   return to_long_iupac_name(tag, nuclide_standard);
    else                    return to_long_pdg_name(tag, particle_standard);
}
std::string to_long_standard_name(
    const Pantag tag,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0),
    const names::Nuclides::Standard nuclide_standard = names::Nuclides::Standard(0))
{
    return to_long_standard_name(tag, nuclide_standard, particle_standard);
}

// ================================================================================================

// TODO
// nuclides:
// -- elementals: just the atomic symbol
// -- non-elementals: <atomic symbol>-<atomic mass number>[[m|e]<index>]
// particles:
// -- try matching the list: scan over names::Particles::count and compare against get_symbol

PORTABLE_FUNCTION constexpr std::size_t read_number(const char* &ptr) {
    std::size_t num = 0; 
    while(true) {
        switch(*ptr) {
        case '0': num = 10 * num;     break; 
        case '1': num = 10 * num + 1; break;
        case '2': num = 10 * num + 2; break;
        case '3': num = 10 * num + 3; break;
        case '4': num = 10 * num + 4; break;
        case '5': num = 10 * num + 5; break;
        case '6': num = 10 * num + 6; break;
        case '7': num = 10 * num + 7; break;
        case '8': num = 10 * num + 8; break;
        case '9': num = 10 * num + 9; break;
        default: return num;
        }
        ++ptr; 
    } 
} 

PORTABLE_FUNCTION constexpr Pantag parse parse_short_nuclide(
    const std::string_view short_name, const std::size_t hyphen_index)
{
    const auto Z = match_nuclide_symbol(short_name.substr(0, n));
    // TODO: What if there's an invalid nuclide symbol?
    const char * ptr = short_name.data() + n + 1;
    const std::size_t A = read_number(ptr);
    const char c = *(ptr++);
    const std::size_t S = (c != '\0' ? read_number(ptr) : 0);
    assert(*ptr == '\0');
    switch(c) {
    case 'm':
        return Pantag(Pantag::PNType::nuclide, Pantag::Mode::standard, Z, A, Pantag::Index::metastable, S);
    case 'e':
        return Pantag(Pantag::PNType::nuclide, Pantag::Mode::standard, Z, A, Pantag::Index::excitation, S);
    default:
        return Pantag(Pantag::PNType::nuclide, Pantag::Mode::standard, Z, A);
    };
}

PORTABLE_FUNCTION constexpr Pantag from_short_standard_name(const std::string_view short_name)
{
    // Test particles
    const auto pindex = match_particle_symbol(short_name);
    if (/*TODO: How to tell if pindex matched correctly?*/) {
        return Pantag(Pantag::PNType::particle, Pantag::Mode::standard, pindex);
    }
    for (std::size_t index = 0; index < names::Particles::count; ++index) {
        if (names::Particles::get_symbol(index) == short_name) {
            // TODO: Return the Pantag for the particle
        }
    }
    // No particles matched, so assume a nuclide
    // std::find not constexpr until C++20
    for (std::size_t n = 0; n < short_name.size(); ++n) {
        if (short_name[n] == '-') {
            return parse_short_nuclide(short_name, n);
        }
    }
    // Did not find '-', so assume an elemental
    const auto Z = match_nuclide_symbol(short_name);
    return Pantag(Pantag::PNType::nuclide, Pantag::Mode::standard, Z, Pantag::elemental);
}

// ================================================================================================

// TODO
Pantag from_long_standard_string(std::string) {}

// ================================================================================================

} // end namespace nautilus::tag

#endif // #ifndef NAUTILUS_FORMAT_STANDARD_HPP
