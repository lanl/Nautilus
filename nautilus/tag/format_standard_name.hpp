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

PORTABLE_FUNCTION constexpr void append(char*& buf, std::size_t n) {
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
    auto name = null_array<SHORT_LEN>();
    char* ptr = name.data();
    detail::append(ptr, Nuclides::get_symbol(tag.get_atomic_number()));
    detail::append(ptr, '-');
    detail::append(ptr, tag.get_atomic_mass_number());
    if (!tag.is_ground()) {
        detail::append(ptr, tag.has_metastable_index() ? 'm', 'e');
        detail::append(ptr, tag.get_index());
    }
    return name;
}
std::string to_short_iupac_name(const Pantag tag)
{
    return to_short_iupac_name_portable(tag).data();
}

// ================================================================================================

PORTABLE_FUNCTION constexpr std::array<char, LONG_LEN> to_long_iupac_name_portable(
    const Pantag tag, const Nuclides::Standard standard = Nuclides::Standard(0))
{
    assert(tag.is_nuclide() && tag.is_standard());
    auto name = null_array<LONG_LEN>();
    char* ptr = name.data();
    detail::append(ptr, Nuclides::get_name(tag.get_atomic_number(), standard));
    detail::append(ptr, '-');
    detail::append(ptr, tag.get_atomic_mass_number());
    if (!tag.is_ground()) {
        detail::append(ptr, tag.has_metastable_index() ? 'm' : 'e');
        detail::append(ptr, tag.get_index());
    }
    return name;
}
std::string to_long_iupac_name(
    const Pantag tag, const Nuclides::Standard standard = Nuclides::Standard(0))
{
    return to_long_iupac_name_portable(tag, standard).data();
}

// ================================================================================================

PORTABLE_FUNCTION constexpr std::array<char, SHORT_LEN> to_short_pdg_name_portable(const Pantag tag)
{
    assert(tag.is_particle() && tag.is_standard());
    auto name = null_array<SHORT_LEN>();
    char* ptr = name.data();
    detail::append(ptr, Particle::get_symbol());
    return name;
}
std::string to_short_pdg_name(const Pantag tag)
{
    return to_short_pdg_name_portable(tag);
}

// ================================================================================================

PORTABLE_FUNCTION constexpr std::array<char, LONG_LEN> to_long_pdg_name_portable(
    const Pantag tag, const Particles::Standard standard = Particles::Standard(0))
{
    assert(tag.is_particle() && tag.is_standard());
    auto name = null_array<LONG_LEN>();
    char* ptr = name.data();
    detail::append(ptr, Particle::get_name(standard));
    return name;
}
std::string to_long_pdg_name(const Pantag tag, const Particles::Standard standard)
{
    return to_long_pdg_name_portable(tag, standard);
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
    const Nuclides::Standard nuclide_standards,
    const Particles::Standard particle_standard = Particles::Standard(0))
{
    if (tag.is_nuclide())   return to_long_iupac_name_portable(tag, nuclide_standard);
    else                    return to_long_pdg_name_portable(tag, particle_standard);
}
std::array<char, LONG_LEN> to_long_standard_name_portable(
    const Pantag tag,
    const Particles::Standard particle_standard = Particles::Standard(0),
    const Nuclides::Standard nuclide_standards = Nuclides::Standard(0))
{
    return to_long_standard_name_portable(nuclide_standard, particle_standard);
}

// ================================================================================================

// Variations of the same thing so that the user can specify a nuclide standard and/or a particle
// standard, and if they specify both then the order is irrelevant.
std::string to_long_standard_name(
    const Pantag tag,
    const Nuclides::Standard nuclide_standards,
    const Particles::Standard particle_standard = Particles::Standard(0))
{
    if (tag.is_nuclide())   return to_long_iupac_name(tag, nuclide_standard);
    else                    return to_long_pdg_name(tag, particle_standard);
}
std::string to_long_standard_name(
    const Pantag tag,
    const Particles::Standard particle_standard = Particles::Standard(0),
    const Nuclides::Standard nuclide_standards = Nuclides::Standard(0))
{
    return to_long_standard_name(nuclide_standard, particle_standard);
}

// ================================================================================================

// TODO
Pantag from_short_standard_name(std::string) {}

// ================================================================================================

// TODO
Pantag from_long_standard_string(std::string) {}

// ================================================================================================

} // end namespace nautilus::tag

#endif // #ifndef NAUTILUS_FORMAT_STANDARD_HPP
