#ifndef NAUTILUS_FORMAT_STANDARD_HPP
#define NAUTILUS_FORMAT_STANDARD_HPP

#include "nautilus/names.hpp"
#include "nautilus/tag/pantag.hpp"

#include "ports-of-call/portability.hpp" // TODO: Add PORTABLE_FUNCTION annotations

#include <cassert>
// TODO: We don't want std::string, but something that runs on GPUs (std::string_view?)
#include <string>

// TODO: Should some/all of these be constexpr?

namespace nautilus::tag {

// ================================================================================================

// longest IUPAC symbol: 10 characters
// -- 2 characters for element name
// -- 4 characters for atomic mass number ("-###")
// -- 4 characters for excitation/metastable suffix ("[em]###")
std::string to_short_iupac_name(const Pantag tag)
{
    assert(tag.is_nuclide() && tag.is_standard());
    auto ci_str = [](const char c, const auto n) { return std::to_string(c) + std::to_string(n); };
    const std::string name = Nuclides::get_symbol(tag.get_atomic_number());
    const std::string isotope = ci_str('-', tag.get_atomic_mass_number());
    const std::string suffix =
        (tag.is_ground() ? "" : ci_str(tag.has_metastable_index() ? 'm' : 'e', tag.get_index()));
    return name + isotope + suffix;
}

// ================================================================================================

// longest IUPAC name: 20 characters
// -- 12 characters for element name
// --  4 characters for atomic mass number ("-###")
// --  4 characters for excitation/metastable suffix ("[em]###")
std::string to_long_iupac_name(const Pantag tag, const standards_flag_t standard)
{
    assert(tag.is_nuclide() && tag.is_standard());
    auto ci_str = [](const char c, const auto n) { return std::to_string(c) + std::to_string(n); };
    const std::string name = Nuclides::get_name(tag.get_atomic_number(), standard);
    const std::string isotope = ci_str('-', tag.get_atomic_mass_number());
    const std::string suffix =
        (tag.is_ground() ? "" : ci_str(tag.has_metastable_index() ? 'm' : 'e', tag.get_index()));
    return name + isotope + suffix;
}

// ================================================================================================

std::string to_short_pdg_name(const Pantag tag)
{
    assert(tag.is_particle() && tag.is_standard());
    return Particles::get_symbol();
}

// ================================================================================================

std::string to_long_pdg_name(const Pantag tag, const standards_flag_t standard)
{
    assert(tag.is_particle() && tag.is_standard());
    return Particles::get_name(standard);
}

// ================================================================================================

// For symbols:
// -- nuclides only have a single standard
// -- the symbols for the "alternate" particle convention cannot all be represented in Unicode, so
//    you always get the PDG symbol
std::string to_short_standard_name(const Pantag tag)
{
    if (tag.is_nuclide()) {
        return to_short_iupac_name(tag);
    } else {
        return to_short_pdg_name(tag);
    }
}

// ================================================================================================

// TODO: Need to be able to specify a standard.  At this point in Nautilus, you can operate on tags
//       without caring if you have a nuclide or a particle.  So how do we specify the standards?
//    -- Possibility: Pass two standards flags.  If I switch to enums instead of std::size_t (not
//       sure if that's a good or bad idea), then I could have four versions and rely on dispatch:
//       both standards default, default for nuclides and non-default for particles, non-default
//       for nuclides and default for particles, non-default for both.
//    -- Possibility: Pack the standards flags.  This involves shifting one or both flags so that
//       they don't overlap, and then the user can pass in something like
//       "Nuclides::Standard::American | Particles::Standard::PDG".
//    -- Possibility: Create translation routines: one to combine a Nuclides flag and a Particles
//       flag, one to extract the Nuclides flag, and one to extract the Particles flag.  Something
//       like "flag = standards(Nuclides::Standard::American, Particles::Standard::PDG)",
//       "nuclide_standard(flag)", and "particle_standard(flag)"?
std::string to_long_standard_name(const Pantag tag, const standards_flag_t standards)
{
    if (tag.is_nuclide()) {
        return to_long_iupac_name(tag, nuclide_standard(standards));
    } else {
        return to_long_pdg_name(tag, particle_standard(standards));
    }
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
