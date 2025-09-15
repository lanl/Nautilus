#ifndef NAUTILUS_FORMAT_STANDARD_HPP
#define NAUTILUS_FORMAT_STANDARD_HPP

#include "nautilus/entity_tag/names.hpp"
#include "nautilus/entity_tag/entity_tag.hpp"
#include "nautilus/util/string_processing.hpp"

#include "ports-of-call/portability.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <string_view>

namespace nautilus::entity_tag {

// ================================================================================================

namespace detail {

const std::string invalid = "unknown";

inline EntityTag parse_nuclide(const std::string_view name, const std::size_t hyphen_index)
{
    auto tokens = tokenize_nuclide(name);
    // Remove the hyphen
    assert(tokens[0].size() == hyphen_index + 1);
    tokens[0] = tokens[0].substr(0, hyphen_index);
    // Valid name or symbol?
    const auto Z = names::Nuclides::find_index(tokens[0]);
    if (Z == names::Nuclides::not_found) {
        return EntityTag(EntityTag::unknown);
    }
    // Atomic mass number (elementals are handled separately)
    if (tokens[1].size() == 0) {
        return EntityTag(EntityTag::unknown);
    }
    const auto A = std::stoi(tokens[1]);
    // Excited state annotation ("m" or "e" alone defaults to "m1" or "e1")
    const auto S = (tokens[2].size() > 1 ? std::stoi(tokens[2].substr(1)) : 1);
    switch (tokens[2][0]) {
    case '\0': [[fallthrough]];
    case 'g': return EntityTag(Z, A); break;
    case 'm': return EntityTag(Z, A, S); break;
    default: return EntityTag(EntityTag::unknown);
    }
}

inline std::string to_short_standard_nuclide_name(const EntityTag tag)
{
    assert(tag.is_nuclide());
    std::string name;
    const auto Z = tag.get_atomic_number();
    if ((Z == 0) || (Z > names::Nuclides::count)) {
        return invalid;
    }
    name.append(names::Nuclides::get_symbol(Z));
    if (!tag.is_elemental()) {
        name.append("-");
        name.append(std::to_string(tag.get_atomic_mass_number()));
        if (!tag.is_ground()) {
            name.append(1, 'm');
            name.append(std::to_string(tag.get_metastable_index()));
        }
    }
    return name;
}

inline std::string to_long_standard_nuclide_name(
    const EntityTag tag, const names::Nuclides::Standard standard = names::Nuclides::Standard(0))
{
    assert(tag.is_nuclide());
    std::string name;
    if (tag.is_elemental()) {
        name.append("elemental ");
    }
    const auto Z = tag.get_atomic_number();
    if ((Z == 0) || (Z > names::Nuclides::count)) {
        return invalid;
    }
    name.append(names::Nuclides::get_name(Z, standard));
    if (!tag.is_elemental()) {
        name.append("-");
        name.append(std::to_string(tag.get_atomic_mass_number()));
        if (!tag.is_ground()) {
            name.append(1, 'm');
            name.append(std::to_string(tag.get_metastable_index()));
        }
    }
    return name;
}

inline std::string to_short_standard_particle_name(const EntityTag tag)
{
    assert(tag.is_particle());
    const auto index = tag.get_particle_index();
    if (index >= names::Particles::count) {
        return invalid;
    }
    return std::string(names::Particles::get_symbol(index));
}

inline std::string to_long_standard_particle_name(
    const EntityTag tag, const names::Particles::Standard standard = names::Particles::Standard(0))
{
    assert(tag.is_particle());
    const auto index = tag.get_particle_index();
    if (index >= names::Particles::count) {
        return invalid;
    }
    return std::string(names::Particles::get_name(index, standard));
}

} // namespace detail

// ================================================================================================

// For symbols:
// -- nuclides only have a single standard
// -- the symbols for the "alternate" particle convention cannot all be represented in Unicode, so
//    you always get the PDG symbol
inline std::string to_short_standard_name(const EntityTag tag)
{
    if (tag.is_nuclide()) {
        return detail::to_short_standard_nuclide_name(tag);
    } else if (tag.is_particle()) {
        return detail::to_short_standard_particle_name(tag);
    } else {
        return detail::invalid;
    }
}

// ================================================================================================

// Variations of the same thing so that the user can specify a nuclide standard and/or a particle
// standard, and if they specify both then the order is irrelevant.
inline std::string to_long_standard_name(
    const EntityTag tag,
    const names::Nuclides::Standard nuclide_standard,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0))
{
    if (tag.is_nuclide()) {
        return detail::to_long_standard_nuclide_name(tag, nuclide_standard);
    } else if (tag.is_particle()) {
        return detail::to_long_standard_particle_name(tag, particle_standard);
    } else {
        return detail::invalid;
    }
}
inline std::string to_long_standard_name(
    const EntityTag tag,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0),
    const names::Nuclides::Standard nuclide_standard = names::Nuclides::Standard(0))
{
    return to_long_standard_name(tag, nuclide_standard, particle_standard);
}

// ================================================================================================

// Because the long and short names are so closely related, and because of how Nuclides::find_index
// and Particles::find_index are implemented, having from_short_standard_name and
// from_long_standard_name would be redundant because they would be identical and both would work
// for both short and long names.
inline EntityTag from_standard_name(const std::string_view name)
{
    // Check if we have a known particle
    const auto pindex = names::Particles::find_index(name);
    if (pindex != names::Particles::not_found) {
        return EntityTag(pindex);
    }
    // Not a particle, so assume a nuclide
    for (std::size_t n = 0; n < name.size(); ++n) {
        if (name[n] == '-') {
            // A hyphen means we have a specific nuclide and not an elemental
            return detail::parse_nuclide(name, n);
        }
    }
    // Did not find '-', so assume an elemental
    const auto pos = name.find_last_of(' ');
    const std::string_view name0 = (pos == std::string_view::npos ? name : name.substr(pos + 1));
    const auto Z = names::Nuclides::find_index(name0);
    if (Z == names::Nuclides::not_found) {
        return EntityTag(EntityTag::unknown);
    }
    return EntityTag(Z, EntityTag::elemental);
}

// ================================================================================================

} // end namespace nautilus::entity_tag

#endif // #ifndef NAUTILUS_FORMAT_STANDARD_HPP
