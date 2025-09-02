#ifndef NAUTILUS_FORMAT_STANDARD_HPP
#define NAUTILUS_FORMAT_STANDARD_HPP

#include "nautilus/tag/names.hpp"
#include "nautilus/tag/pantag.hpp"

#include "ports-of-call/portability.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <string_view>

namespace nautilus::tag {

// ================================================================================================

namespace detail {

inline Pantag parse_nuclide(const std::string_view name, const std::size_t hyphen_index)
{
    // Split into tokens
    auto is_number = [](const char s) {
        switch (s) {
        case '0': [[fallthrough]];
        case '1': [[fallthrough]];
        case '2': [[fallthrough]];
        case '3': [[fallthrough]];
        case '4': [[fallthrough]];
        case '5': [[fallthrough]];
        case '6': [[fallthrough]];
        case '7': [[fallthrough]];
        case '8': [[fallthrough]];
        case '9':
            return true;
            break;
            default : return false;
        }
    };
    const auto token0 = name.substr(0, hyphen_index);
    const auto it1 = std::find_if(name.begin() + hyphen_index + 1, name.end(), is_number);
    const auto it2 = std::find_if_not(it1, name.end(), is_number);
    const auto token1 = std::string(it1, it2);
    const auto token2 = std::string(it2, name.end());
    // Parse
    const auto Z = names::Nuclides::find_index(token0);
    const auto A = std::stoi(token1);
    assert((token2[0] == 'g') || (token2[0] == 'm') || (token2[0] == 'e') || (token2[0] == '\0'));
    const auto S = (token2.size() > 1 ? std::stoi(token2.substr(1)) : 1);
    switch (token2[0]) {
    case 'm': return Pantag(Z, A, Pantag::Index::metastable, S); break;
    case 'e': // TODO: Check with Wim: Is this standard?
              //    -- Maybe this is a detail of the _format_ and not of the tag?  That is, if I
              //       convert to standard format (assuming "e#" isn't really standard), then we
              //       always go to "m#"), but if we go to another format (Where is Wim using this?
              //       NJOY?) then we assume it's an excitation state and go to "e#"?
        return Pantag(Z, A, Pantag::Index::excitation, S);
        break;
    case 'g': [[fallthrough]];
    case '\0':
        return Pantag(Z, A);
        break;
    }
}

inline std::string to_short_standard_nuclide_name(const Pantag tag)
{
    assert(tag.is_nuclide() && tag.is_standard());
    std::string name;
    name.append(names::Nuclides::get_symbol(tag.get_atomic_number()));
    if (!tag.is_elemental()) {
        name.append("-");
        name.append(std::to_string(tag.get_atomic_mass_number()));
        if (!tag.is_ground()) {
            name.append(tag.has_metastable_index() ? "m" : "e");
            name.append(std::to_string(tag.get_index()));
        }
    }
    return name;
}

inline std::string to_long_standard_nuclide_name(
    const Pantag tag, const names::Nuclides::Standard standard = names::Nuclides::Standard(0))
{
    assert(tag.is_nuclide() && tag.is_standard());
    std::string name;
    if (tag.is_elemental()) {
        name.append("elemental ");
    }
    name.append(names::Nuclides::get_name(tag.get_atomic_number(), standard));
    if (!tag.is_elemental()) {
        name.append("-");
        name.append(std::to_string(tag.get_atomic_mass_number()));
        if (!tag.is_ground()) {
            name.append(tag.has_metastable_index() ? "m" : "e");
            name.append(std::to_string(tag.get_index()));
        }
    }
    return name;
}

inline std::string to_short_standard_particle_name(const Pantag tag)
{
    assert(tag.is_particle() && tag.is_standard());
    return std::string(names::Particles::get_symbol(tag.get_particle_index()));
}

inline std::string to_long_standard_particle_name(
    const Pantag tag, const names::Particles::Standard standard = names::Particles::Standard(0))
{
    assert(tag.is_particle() && tag.is_standard());
    return std::string(names::Particles::get_name(tag.get_particle_index(), standard));
}

} // namespace detail

// ================================================================================================

// For symbols:
// -- nuclides only have a single standard
// -- the symbols for the "alternate" particle convention cannot all be represented in Unicode, so
//    you always get the PDG symbol
inline std::string to_short_standard_name(const Pantag tag)
{
    if (tag.is_nuclide())
        return detail::to_short_standard_nuclide_name(tag);
    else
        return detail::to_short_standard_particle_name(tag);
}

// ================================================================================================

// Variations of the same thing so that the user can specify a nuclide standard and/or a particle
// standard, and if they specify both then the order is irrelevant.
inline std::string to_long_standard_name(
    const Pantag tag,
    const names::Nuclides::Standard nuclide_standard,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0))
{
    if (tag.is_nuclide())
        return detail::to_long_standard_nuclide_name(tag, nuclide_standard);
    else
        return detail::to_long_standard_particle_name(tag, particle_standard);
}
inline std::string to_long_standard_name(
    const Pantag tag,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0),
    const names::Nuclides::Standard nuclide_standard = names::Nuclides::Standard(0))
{
    return to_long_standard_name(tag, nuclide_standard, particle_standard).data();
}

// ================================================================================================

// Because the long and short names are so closely related, and because of how Nuclides::find_index
// and Particles::find_index are implemented, having from_short_standard_name and
// from_long_standard_name would be redundant because they would be identical and both would work
// for both short and long names.
inline Pantag from_standard_name(const std::string_view short_name)
{
    // Check if we have a known particle
    const auto pindex = names::Particles::find_index(short_name);
    if (pindex != names::Particles::not_found) {
        return Pantag(pindex);
    }
    // Not a particle, so assume a nuclide
    for (std::size_t n = 0; n < short_name.size(); ++n) {
        if (short_name[n] == '-') {
            // A hyphen means we have a specific nuclide and not an elemental
            return detail::parse_nuclide(short_name, n);
        }
    }
    // Did not find '-', so assume an elemental
    assert(short_name.size() > 10);
    for (std::size_t n = 0; n < short_name.size(); ++n) {
        if (short_name[n] == ' ') {
            // A space means we have a "long name" for an elemental
            const auto Z = names::Nuclides::find_index(short_name.substr(n + 1));
            assert(Z != names::Nuclides::not_found);
            return Pantag(Z, Pantag::elemental);
        }
    }
    // Did not find ' ', so assume a "short name" for an elemental
    const auto Z = names::Nuclides::find_index(short_name);
    assert(Z != names::Nuclides::not_found);
    return Pantag(Z, Pantag::elemental);
}

// ================================================================================================

} // end namespace nautilus::tag

#endif // #ifndef NAUTILUS_FORMAT_STANDARD_HPP
