#ifndef NAUTILUS_FORMAT_STANDARD_HPP
#define NAUTILUS_FORMAT_STANDARD_HPP

#include "nautilus/entity_tag/entity_tag.hpp"
#include "nautilus/entity_tag/names.hpp"
#include "nautilus/util/string_processing.hpp"

#include "ports-of-call/portability.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <string_view>

namespace nautilus::entity_tag {

// ================================================================================================

const std::string invalid_standard_symbol = "unknown";

// For symbols:
// -- nuclides only have a single standard
// -- the symbols for the "alternate" particle convention cannot all be represented in Unicode, so
//    you always get the PDG symbol
inline std::string to_standard_symbol(const EntityTag tag)
{
    if (tag.is_user()) {
        std::string name(9, ' ');
        sprintf(name.data(), "U:%07X", static_cast<unsigned int>(tag.get_user_data()));
        return name;
    } else if (tag.is_nuclide()) {
        const auto Z = tag.get_atomic_number();
        if ((Z == 0) || (Z > names::Nuclides::count)) {
            return invalid_standard_symbol;
        }
        std::string name(names::Nuclides::get_symbol(Z));
        name.append("-");
        name.append(std::to_string(tag.get_atomic_mass_number()));
        if (!tag.is_ground()) {
            name.append(1, 'm');
            name.append(std::to_string(tag.get_metastable_index()));
        }
        return name;
    } else if (tag.is_elemental()) {
        const auto Z = tag.get_atomic_number();
        if ((Z == 0) || (Z > names::Nuclides::count)) {
            return invalid_standard_symbol;
        }
        return std::string(names::Nuclides::get_symbol(Z));
    } else if (tag.is_particle()) {
        const auto index = tag.get_particle_index();
        if (index >= names::Particles::count) {
            return invalid_standard_symbol;
        }
        return std::string(names::Particles::get_symbol(index));
    } else {
        return invalid_standard_symbol;
    }
}

// ================================================================================================

const std::string invalid_standard_name = "unknown";

// Variations of the same thing so that the user can specify a nuclide standard and/or a particle
// standard, and if they specify both then the order is irrelevant.
inline std::string to_standard_name(
    const EntityTag tag,
    const names::Nuclides::Standard nuclide_standard,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0))
{
    if (tag.is_user()) {
        std::string name(21, ' ');
        sprintf(name.data(), "user entity 0x%07X", static_cast<unsigned int>(tag.get_user_data()));
        return name;
    } else if (tag.is_nuclide()) {
        const auto Z = tag.get_atomic_number();
        if ((Z == 0) || (Z > names::Nuclides::count)) {
            return invalid_standard_name;
        }
        std::string name(names::Nuclides::get_name(Z, nuclide_standard));
        name.append("-");
        name.append(std::to_string(tag.get_atomic_mass_number()));
        if (!tag.is_ground()) {
            name.append(1, 'm');
            name.append(std::to_string(tag.get_metastable_index()));
        }
        return name;
    } else if (tag.is_elemental()) {
        std::string name = "elemental ";
        const auto Z = tag.get_atomic_number();
        if ((Z == 0) || (Z > names::Nuclides::count)) {
            return invalid_standard_name;
        }
        name.append(names::Nuclides::get_name(Z, nuclide_standard));
        return name;
    } else if (tag.is_particle()) {
        const auto index = tag.get_particle_index();
        if (index >= names::Particles::count) {
            return invalid_standard_name;
        }
        return std::string(names::Particles::get_name(index, particle_standard));
    } else {
        return invalid_standard_name;
    }
}
inline std::string to_standard_name(
    const EntityTag tag,
    const names::Particles::Standard particle_standard = names::Particles::Standard(0),
    const names::Nuclides::Standard nuclide_standard = names::Nuclides::Standard(0))
{
    return to_standard_name(tag, nuclide_standard, particle_standard);
}

// ================================================================================================

// Because the long and short names are so closely related, and because of how Nuclides::find_index
// and Particles::find_index are implemented, having from_standard_symbol and
// from_standard_name would be redundant because they would be identical and both would work
// for both short and long names.
inline EntityTag from_standard_name_or_symbol(
        const std::string_view name, const std::string_view user_prefix)
{
    // Check if we have user data
    if (name.substr(0, user_prefix.size()) == user_prefix) {
        assert(name.size() == user_prefix.size() + 7);
        const std::string s(name.substr(user_prefix.size()));
        const unsigned int data = std::stoi(s, nullptr, 16);
        assert(data != 0x1FFFFFF);
        return EntityTag(EntityTag::user, data);
    }
    // Check if we have a known particle
    const auto pindex = names::Particles::find_index(name);
    if (pindex != names::Particles::not_found) {
        return EntityTag(pindex);
    }
    // Not a particle: try nuclide next
    // -- nuclides will have a hyphen, while elementals won't
    const auto n = name.find_first_of('-');
    if (n != std::string_view::npos) {
        // A hyphen means we have a specific nuclide and not an elemental
        auto tokens = tokenize_nuclide(name);
        // Remove the hyphen
        assert(tokens[0].size() == n + 1);
        tokens[0] = tokens[0].substr(0, n);
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
        // Excited state annotation ("m" alone defaults to "m1")
        const auto S = (tokens[2].size() > 1 ? std::stoi(tokens[2].substr(1)) : 1);
        switch (tokens[2][0]) {
        case '\0': [[fallthrough]];
        case 'g': return EntityTag(Z, A); break;
        case 'm': return EntityTag(Z, A, S); break;
        default: return EntityTag(EntityTag::unknown);
        }
    }
    // Did not find '-', so assume an elemental
    const auto pos = name.find_last_of(' ');
    const std::string_view name0 = (pos == std::string_view::npos ? name : name.substr(pos + 1));
    const auto Z = names::Nuclides::find_index(name0);
    if (Z == names::Nuclides::not_found) {
        return EntityTag(EntityTag::unknown);
    }
    return EntityTag(Z);
}

// Aliases for consistency
inline EntityTag from_standard_name(const std::string_view name)
{
    return from_standard_name_or_symbol(name, "user entity 0x");
}
inline EntityTag from_standard_symbol(const std::string_view name)
{
    return from_standard_name_or_symbol(name, "U:");
}

// ================================================================================================

} // end namespace nautilus::entity_tag

#endif // #ifndef NAUTILUS_FORMAT_STANDARD_HPP
