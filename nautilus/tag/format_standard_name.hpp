#ifndef NAUTILUS_FORMAT_STANDARD_HPP
#define NAUTILUS_FORMAT_STANDARD_HPP

#include "nautilus/tag/names.hpp"
#include "nautilus/tag/pantag.hpp"
#include "nautilus/tag/tokenize.hpp"

#include "ports-of-call/portability.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <string_view>

// TODO: What to return if a tag is not representable in standard format?
//    -- unknown tag
//    -- user tag

namespace nautilus::tag {

// ================================================================================================

namespace detail {

inline Pantag parse_nuclide(const std::string_view name, const std::size_t hyphen_index)
{
    auto tokens = tokenize_nuclide(name);
    // Remove the hyphen
    assert(tokens[0].size() == hyphen_index + 1);
    tokens[0] = tokens[0].substr(0, hyphen_index);
    // Valid name or symbol?
    const auto Z = names::Nuclides::find_index(tokens[0]);
    if (Z == names::Nuclides::not_found) {
        return Pantag(Pantag::unknown);
    }
    // Atomic mass number (elementals are handled separately)
    if (tokens[1].size() == 0) {
        return Pantag(Pantag::unknown);
    }
    const auto A = std::stoi(tokens[1]);
    // Excited state annotation ("m" or "e" alone defaults to "m1" or "e1")
    const auto c = tokens[2][0];
    assert((c == 'g') || (c == 'm') || (c == 'e') || (c == '\0'));
    const auto S = (tokens[2].size() > 1 ? std::stoi(tokens[2].substr(1)) : 1);
    switch (c) {
    case 'm': return Pantag(Z, A, Pantag::Index::metastable, S); break;
    case 'e': // TODO: Check with Wim: Is this standard?
              //    -- Maybe this is a detail of the _format_ and not of the tag?  That is, if I
              //       convert to standard format (assuming "e#" isn't really standard), then we
              //       always go to "m#"), but if we go to another format (Where is Wim using this?
              //       NJOY?) then we assume it's an excitation state and go to "e#"?
        return Pantag(Z, A, Pantag::Index::excitation, S);
        break;
    case 'g': [[fallthrough]];
    case '\0': return Pantag(Z, A); break;
    default: return Pantag(Pantag::unknown);
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
    // TODO: Add a default of returning "unknown" if any parsing step fails
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
