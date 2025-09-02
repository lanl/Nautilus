#ifndef NAUTILUS_FORMAT_IC_HPP
#define NAUTILUS_FORMAT_IC_HPP

// TODO: What should happen if a Pantag doesn't map to the IC chemsym format?

#include <algorithm>
#include <cmath>

#include "nautilus/tag/pantag.hpp"
#include "nautilus/tag/string_processing.hpp"

namespace nautilus::tag {

// ================================================================================================
// IC chemsym

inline std::string to_IC_chemsym(Pantag tag)
{
    if (tag.is_nuclide()) {
        const auto Z = tag.get_atomic_number();
        std::string result(names::Nuclides::get_symbol(Z));
        result[0] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[0])));
        // lawrencium is uses a pre-standard symbol and was never updated after standardization
        if (result == "lr") {
            result = "lw";
        }
        if (tag.is_elemental()) {
            // elementals are just the atomic symbol
            return result;
        } else {
            // append the atomic mass number
            const auto A = tag.get_atomic_mass_number();
            result.append(std::to_string(A));
            // append the metastable state if not in the ground state
            // -- can't just check tag.is_ground() because Am-242g and Am-242m1 are swapped
            assert(tag.has_metastable_index());
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
        }
    } else {
        assert(tag.is_particle());
        const auto pidx = tag.get_particle_index();
        switch (pidx) {
        case (nautilus::tag::names::photon): return "g0"; break;
        case (nautilus::tag::names::neutron): return "nt1"; break;
        default: assert(false); return "?";
        }
    }
}

inline Pantag from_IC_chemsym(const std::string_view sv0)
{
    const std::string sv(sv0.substr(0, sv0.find('.')));
    if ((sv == "g") || (sv == "g0")) {
        return Pantag(nautilus::tag::names::photon);
    } else if (sv == "nt1") {
        return Pantag(nautilus::tag::names::neutron);
    } else if ((sv == "am242") || (sv == "am242g")) {
        // Am-242g and Am-242m1 are swapped in NDI
        return Pantag(95, 242, Pantag::Index::metastable, 1);
    } else if ((sv == "am42") || (sv == "am042") || (sv == "am242m1")) {
        // Am-242g and Am-242m1 are swapped in NDI
        return Pantag(95, 242);
    }
    // If we fall through to here, we assume this is a nuclide
    const auto tokens = tokenize_nuclide(sv);
    // Get the atomic number
    auto symbol = tokens[0];
    symbol[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(symbol[0])));
    if (symbol == "Lw") { // lawrencium is non-standard
        symbol = "Lr";
    }
    const auto Z = names::Nuclides::find_index(symbol);
    if (Z == names::Nuclides::not_found) {
        return Pantag(Pantag::unknown);
    }
    // Get the atomic mass number
    if (tokens[1].size() == 0) {
        return Pantag(Z, Pantag::elemental);
    }
    const auto A = std::stoi(tokens[1]);
    // Get the excitation index
    const auto c = tokens[2][0];
    assert((c == 'g') || (c == 'm') || (c == '\0'));
    switch(c) {
    case 'g': [[fallthrough]];
    case '\0': return Pantag(Z, A); break;
    case 'm':
        return Pantag(Z, A, Pantag::Index::metastable, std::stoi(tokens[2].substr(1)));
        break;
    default: return Pantag(Pantag::unknown);
    }
}

// ================================================================================================

} // end namespace nautilus::tag

#endif // $ifndef NAUTILUS_FORMAT_IC_HPP
