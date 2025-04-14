#ifndef NAUTILUS_ZAIDTOOLS_HPP
#define NAUTILUS_ZAIDTOOLS_HPP

#include <cassert>
#include <string>
#include <vector>

namespace nautilus {

// The ``chemsym_to_zaid`` function accepts a string in a format known as the "chemsym" format,
// because it is related to (but not the same as) the IUPAC chemical symbol.  It currently only
// handles a subset of the full chemsym format.  Given a chemsym, it will return an integer with
// the corresponding SZA value.  The names "zaid" and "SZA" were incorrectly used interchangeably
// when this function was initially written.
//    There should be a corresponding zaid-to-chemsym conversion routine.  However, this will
// probably be superseded by the new format and its associated translation routines.
inline int chemsym_to_zaid(std::string const & chemsym)
{
    if (chemsym == "g") {
        // special case: gammas / photons
        return 0;
    } else if (chemsym == "n") {
        // special case: neutrons
        return 1;
    } else if (chemsym == "p") {
        // special case: protons / H-1
        return 1001;
    } else if (chemsym == "d") {
        // special case: deuterons / H-2
        return 1002;
    } else if (chemsym == "t") {
        // special case: tritons / H-3
        return 1003;
    } else if (chemsym == "a") {
        // special case: alpha / He-4
        return 2004;
    } else {
        // split into element symbol and atomic mass number
        auto const pos = chemsym.find_first_of("0123456789");
        auto element_symbol = chemsym.substr(0, pos);
        auto atomic_mass_number = chemsym.substr(pos);
        // look-up table to convert element symbol to atomic number
        std::vector<std::string> const elements{
            "h",  "he", "li", "be", "b",  "c",  "n",  "o",  "f",  "ne", "na", "mg", "al", "si",
            "p",  "s",  "cl", "ar", "k",  "ca", "sc", "ti", "v",  "cr", "mn", "fe", "co", "ni",
            "cu", "zn", "ga", "ge", "as", "se", "br", "kr", "rb", "sr", "y",  "zr", "nb", "mo",
            "tc", "ru", "rh", "pd", "ag", "cd", "in", "sn", "sb", "te", "i",  "xe", "cs", "ba",
            "la", "ce", "pr", "nd", "pm", "sm", "eu", "gd", "tb", "dy", "ho", "er", "tm", "yb",
            "lu", "hf", "ta", "w",  "te", "os", "ir", "pt", "au", "hg", "tl", "pb", "bi", "po",
            "at", "rn", "fr", "ra", "ac", "th", "pa", "u",  "np", "pu", "am", "cm", "bk", "cf",
            "es", "fm", "md", "no", "lr", "rf", "db", "sg", "bh", "hs", "mt", "ds", "rg", "cn",
            "nh", "fl", "mc", "lv", "ts", "og"};
        int Z = -1;
        for (std::size_t n{0}; n < elements.size(); ++n) {
            if (elements[n] == element_symbol) {
                Z = static_cast<int>(n) + 1;
                break;
            }
        }
        // -- Z < 0 is an error: didn't find the result
        // -- all Z == 0 cases are special cases handled previously
        assert(Z > 0);
        // For historical reasons, Americium has special rules in the NDI format.  We don't yet
        // handle those rules correctly, so we simply forbid Americium at this time.  This will be
        // addressed in the future, but is low priority since we have no reactions that involve
        // Americium as either reactant or product.
        assert(Z != 95);
        // convert string to integer for atomic mass number
        int N = std::stoi(atomic_mass_number);
        // return zaid
        return 1000 * Z + N;
    }
}

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
        append_if_unique(chemsym_to_zaid(chemsym));
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
