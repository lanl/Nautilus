/* zaid tools

A set of operations to process zaids, both nuclide/particle zaids and reaction zaids.

**WARNING**
This code was written before the work of clarifying all the different identifier formats was
complete, and so the names used here are not always consistent with the detailed documentation for
EntityTag.
*/

#ifndef NAUTILUS_ZAIDTOOLS_HPP
#define NAUTILUS_ZAIDTOOLS_HPP

#include "nautilus/entity_tag/format_ndi.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace nautilus {

// The ``half_reaction_zaid_to_chemsym_list`` function accepts a string consisting of either the
// products or reactants of a reaction, written in the format expected for NDI reaction zaids.  It
// will parse the string and return a ``std::vector`` of strings in the chemsym format.  In
// practice, this is not fully consistent with the chemsym format, because there was some early
// confusion about the distinction between two related formats.
inline std::vector<std::string> half_reaction_zaid_to_chemsym_list(std::string const & half_rzaid)
{
    // notes on std::string::npos:
    // -- has a value of -1, but is unsigned so wraps around to largest value (see
    //    https://en.cppreference.com/w/cpp/string/basic_string/npos)
    // -- original.substr(0, npos) copies all of original
    // -- npos + 1 = 0, due to unsigned wrapround, so original.substr(npos + 1) copies all of
    //    original (which is nothing if original is the empty string)
    std::string remainder = half_rzaid;
    std::vector<std::string> chemsym_list;
    std::string::size_type pos;
    do {
        // get next chemsym
        pos = remainder.find('+');
        auto iso_str = remainder.substr(0, pos); // npos = -1 (unsigned wraparound), copies all
        // strip leading multiplicity
        iso_str = iso_str.substr(iso_str.find_first_not_of("0123456789"));
        // append to list
        chemsym_list.push_back(iso_str);
        // cycle
        remainder = remainder.substr(pos + 1); // npos = -1 (unsigned wraparound), npos + 1 == 0
    } while (pos != std::string::npos);
    return chemsym_list;
}

// The ``append_zaids`` function combines the ``half_reaction_zaid_to_chemsym_list`` and
// ``chemsym_to_zaid`` functions, taking a string listing the products or reactants of a reaction
// and returning a ``std::vector`` of SZA values, with repeated entries deduplicated.
inline void append_zaids(std::vector<int> & zaid_list, std::string const & half_rzaid)
{
    auto const chemsym_list = half_reaction_zaid_to_chemsym_list(half_rzaid);
    auto append_if_unique = [&zaid_list](int const zaid) {
        bool unique = true;
        for (std::size_t n{0}; n < zaid_list.size(); ++n) {
            if (zaid_list[n] == zaid) {
                unique = false;
                break;
            }
        }
        if (unique) {
            zaid_list.push_back(zaid);
        }
    };
    for (auto & chemsym : chemsym_list) {
        using namespace nautilus::entity_tag;
        append_if_unique(to_NDI_SZA(from_NDI_short_string(chemsym)));
    }
}

// The ``get_reactant_zaids`` function accepts an NDI reaction zaid as a string, identifies the
// substring with the reactants, then calls ``append_zaids`` and returns the resulting list of
// integer SZA values.  The ``get_product_zaids`` function is the same, but it will extract and
// parse the list of reaction products instead of the reactants.  The ``get_nuclide_zaids``
// function is similar, but it returns the list of all reactants and reaction products.
inline auto get_reactant_zaids(std::string const & rzaid)
{
    auto split_pos = rzaid.find("->");
    assert(split_pos != std::string::npos);
    std::vector<int> zaid_list;
    append_zaids(zaid_list, rzaid.substr(0, split_pos));
    return zaid_list;
}
inline auto get_product_zaids(std::string const & rzaid)
{
    auto split_pos = rzaid.find("->");
    assert(split_pos != std::string::npos);
    std::vector<int> zaid_list;
    append_zaids(zaid_list, rzaid.substr(split_pos + 2));
    return zaid_list;
}
inline auto get_nuclide_zaids(std::string const & rzaid)
{
    auto split_pos = rzaid.find("->");
    assert(split_pos != std::string::npos);
    std::vector<int> zaid_list;
    append_zaids(zaid_list, rzaid.substr(0, split_pos));
    append_zaids(zaid_list, rzaid.substr(split_pos + 2));
    return zaid_list;
}

} // namespace nautilus

#endif // NAUTILUS_ZAIDTOOLS_HPP
