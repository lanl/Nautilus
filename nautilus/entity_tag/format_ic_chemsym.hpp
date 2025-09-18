// TODO: Rename this file to format_ic.hpp.  Update include guards, documentation files,
//       documentation section name.

#ifndef NAUTILUS_FORMAT_IC_CHEMSYM_HPP
#define NAUTILUS_FORMAT_IC_CHEMSYM_HPP

#include <algorithm>
#include <cmath>

#include "nautilus/entity_tag/entity_tag.hpp"
#include "nautilus/util/string_processing.hpp"

namespace nautilus::entity_tag {

// ================================================================================================
// IC chemsym

const std::string invalid_ic_chemsym = "unknown";

inline std::string to_IC_chemsym(EntityTag tag)
{
    if (tag.is_nuclide()) {
        // start with the atomic symbol, all lowercase
        const auto Z = tag.get_atomic_number();
        if ((Z < 1) || (Z > names::Nuclides::count)) {
            return invalid_ic_chemsym;
        }
        std::string result(names::Nuclides::get_symbol(Z));
        result[0] = to_lower(result[0]);
        if (result == "lr") { // lawrencium uses a pre-standard symbol and was never fixed
            result = "lw";
        }
        if (tag.is_elemental()) {
            // elementals are just the atomic symbol
            return result;
        }
        // append the atomic mass number
        const auto A = tag.get_atomic_mass_number();
        result.append(std::to_string(A));
        // append the metastable state if not in the ground state
        // -- can't just check tag.is_ground() because Am-242g and Am-242m1 are swapped
        auto S = tag.get_metastable_index();
        if ((Z == 95) && (A == 242)) {
            if (S == 0) {
                S = 1;
            } else if (S == 1) {
                S = 0;
            }
        }
        if (S > 0) {
            result.append("m");
            result.append(std::to_string(S));
        }
        return result;
    } else if (tag.is_particle()) {
        const auto pidx = tag.get_particle_index();
        switch (pidx) {
        case names::photon: return "g0"; break;
        case names::neutron: return "nt1"; break;
        default: return invalid_ic_chemsym;
        }
    } else /* "unknown" tag */ {
        return invalid_ic_chemsym;
    }
}

inline EntityTag from_IC_chemsym(const std::string_view sv0)
{
    if (sv0 == invalid_ic_chemsym) {
        return EntityTag(EntityTag::unknown);
    }
    // Throw out the optional suffix
    const std::string sv(sv0.substr(0, sv0.find('.')));
    // Check for known particles or special cases
    if ((sv == "g") || (sv == "g0")) {
        return EntityTag(names::photon);
    } else if (sv == "nt1") {
        return EntityTag(names::neutron);
    } else if ((sv == "am242") || (sv == "am242g")) {
        // Am-242g and Am-242m1 are swapped in NDI
        return EntityTag(95, 242, 1);
    } else if ((sv == "am42") || (sv == "am042") || (sv == "am242m1")) {
        // Am-242g and Am-242m1 are swapped in NDI
        return EntityTag(95, 242);
    }
    // If we fall through to here, we assume this is a nuclide
    const auto tokens = tokenize_nuclide(sv);
    // Get the atomic number
    auto symbol = tokens[0];
    symbol[0] = to_upper(symbol[0]);
    if (symbol == "Lw") { // lawrencium is non-standard
        symbol = "Lr";
    }
    const auto Z = names::Nuclides::find_index(symbol);
    if (Z == names::Nuclides::not_found) {
        return EntityTag(EntityTag::unknown);
    }
    // Get the atomic mass number
    if (tokens[1].size() == 0) {
        return EntityTag(Z, EntityTag::elemental);
    }
    const auto A = std::stoi(tokens[1]);
    // Get the excitation index
    switch (tokens[2][0]) {
    case '\0': [[fallthrough]];
    case 'g': return EntityTag(Z, A); break;
    case 'm': return EntityTag(Z, A, std::stoi(tokens[2].substr(1))); break;
    default: return EntityTag(EntityTag::unknown);
    }
}

// ================================================================================================

} // end namespace nautilus::entity_tag

#endif // $ifndef NAUTILUS_FORMAT_IC_CHEMSYM_HPP
