#ifndef NAUTILUS_FORMAT_STANDARD_HPP
#define NAUTILUS_FORMAT_STANDARD_HPP

#include "pantag.hpp"

#include "ports-of-call/portability.hpp"

#include <cassert>
#include <string>

namespace nautilus::tag {

// TODO: IUPAC standard long names versus national standard long names
//      Z       IUPAC       American        Canadian        British
//      13      aluminium   aluminum        aluminum
//      16      sulfur                                      sulphur
//      55      caesium     cesium

// TODO: For particles, should I add LaTeX output?  Then I can fix the "quirks" of things that
//       aren't currently present in Unicode, and I can also provide the alternate convention
//       (where the overline extends over the sign).  This may be added to the list of, "We could
//       add this, but we'll wait for someone to ask for it."

// ================================================================================================

// longest IUPAC symbol: 10 characters
// -- 2 characters for element name
// -- 4 characters for atomic mass number ("-###")
// -- 4 characters for excitation/metastable suffix ("[em]###")
std::string to_short_iupac_name(const Pantag tag)
{
    assert(tag.is_nuclide() && tag.is_standard());
    auto ci_str = [](const char c, const auto n) { return std::to_string(c) + std::to_string(n); };
    const std::string name = get_element(tag.get_atomic_number()).symbol;
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
std::string to_long_iupac_name(const Pantag tag)
{
    assert(tag.is_nuclide() && tag.is_standard());
    auto ci_str = [](const char c, const auto n) { return std::to_string(c) + std::to_string(n); };
    const std::string name = get_element(tag.get_atomic_number()).name;
    const std::string isotope = ci_str('-', tag.get_atomic_mass_number());
    const std::string suffix =
        (tag.is_ground() ? "" : ci_str(tag.has_metastable_index() ? 'm' : 'e', tag.get_index()));
    return name + isotope + suffix;
}

// ================================================================================================

// Warning: These use UTF-8 code points, so the string may not behave as expected (for example,
//          size may report a longer result than the actual number of printed characters).  Using
//          something like wstring instead leads to different complications, so this was the
//          compromise chosen by Nautilus.
// Warning: Subscript mu does not appear to exist in Unicode at this time, so I had to substitute a
//          subscript lowercase m.
// Warning: Subscript uppercase L and S don't appear to exist in Unicode at this time, and they
//          would be more standard for the long and short kaons.  Instead I had to substitute
//          lowercase subscripts.
// Warning: Unicode does not provide the ability to stack a subscript and a superscript vertically,
//          but must place them side-by-side.  For the long and short kaons, we place the
//          superscript first and the subscript after.
std::string to_short_pdg_name(const Pantag tag)
{
    using Pantag::Particle;
    assert(tag.is_particle() && tag.is_standard());
    // clang-format off
    switch (tag.data()) {
    case(photon):                       return "\u03B3";             break;
    case(electron):                     return "e\u207B";            break;
    case(positron):                     return "e\u207A";            break;
    case(electron_neutrino):            return "\u03BD\u2091";       break;
    case(electron_antineutrino):        return "\u03Bd\u0304\u2091"; break;
    case(muon):                         return "\u03BC\u207B";       break;
    case(antimuon):                     return "\u03BC\u0304\u207A"; break;
    case(muon_neutrino):                return "\u03BD\u2098";       break;
    case(muon_antineutrino):            return "\u03BD\u0304\u2098"; break;
    case(neutral_pion):                 return "\u03C0\u2070";       break;
    case(positive_pion):                return "\u03C0\u207A";       break;
    case(negative_pion):                return "\u03C0\u207B";       break;
    case(short_kaon):                   return "K\u2070\u209B";      break;
    case(long_kaon):                    return "K\u2070\u2097";      break;
    case(positive_kaon):                return "K\u207A";            break;
    case(negative_kaon):                return "K\u207B";            break;
    case(neutron):                      return "n";                  break;
    case(antineutron):                  return "n\u0304";            break;
    case(proton):                       return "p";                  break;
    case(antiproton):                   return "p\u0304";            break;
    case(neutral_lambda_baryon):        return "\u039B\u2070";       break;
    case(neutral_lambda_antibaryon):    return "\u039B\u0304\u2070"; break;
    case(positive_sigma_baryon):        return "\u03A3\u207A";       break;
    case(negative_sigma_antibaryon):    return "\u03A3\u0304\u207B"; break;
    case(negative_sigma_baryon):        return "\u03A3\u207B";       break;
    case(positive_sigma_antibaryon):    return "\u03A3\u0304\u207A"; break;
    case(neutral_xi_baryon):            return "\u039E\u2070";       break;
    case(neutral_xi_antibaryon):        return "\u039E\u0304\u2070"; break;
    case(negative_xi_baryon):           return "\u039E\u207B";       break;
    case(positive_xi_antibaryon):       return "\u039E\u0304\u207A"; break;
    case(negative_omega_baryon):        return "\u03A9\u207B";       break;
    case(positive_omega_antibaryon):    return "\u03A9\u0304\u207A"; break;
    case default: assert(false);
    }
    // clang-format on
}

// ================================================================================================

std::string to_long_pdg_name(const Pantag tag)
{
    using Pantag::Particle;
    assert(tag.is_particle() && tag.is_standard());
    // clang-format off
    switch (tag.data()) {
    case(photon):                       return "photon";                    break;
    case(electron):                     return "electron";                  break;
    case(positron):                     return "positron";                  break;
    case(electron_neutrino):            return "electron neutrino";         break;
    case(electron_antineutrino):        return "electron antineutrino";     break;
    case(muon):                         return "muon";                      break;
    case(antimuon):                     return "antimuon";                  break;
    case(muon_neutrino):                return "muon neutrino";             break;
    case(muon_antineutrino):            return "muon antineutrino";         break;
    case(neutral_pion):                 return "neutral pion";              break;
    case(positive_pion):                return "positive pion";             break;
    case(negative_pion):                return "negative pion";             break;
    case(short_kaon):                   return "short kaon";                break;
    case(long_kaon):                    return "long kaon";                 break;
    case(positive_kaon):                return "positive kaon";             break;
    case(negative_kaon):                return "negative kaon";             break;
    case(neutron):                      return "neutron";                   break;
    case(antineutron):                  return "antineutron";               break;
    case(proton):                       return "proton";                    break;
    case(antiproton):                   return "antiproton";                break;
    case(neutral_lambda_baryon):        return "neutral lambda baryon";     break;
    case(neutral_lambda_antibaryon):    return "neutral lambda antibaryon"; break;
    case(positive_sigma_baryon):        return "positive sigma baryon";     break;
    case(negative_sigma_antibaryon):    return "negative sigma antibaryon"; break;
    case(negative_sigma_baryon):        return "negative sigma baryon";     break;
    case(positive_sigma_antibaryon):    return "positive sigma antibaryon"; break;
    case(neutral_xi_baryon):            return "neutral xi baryon";         break;
    case(neutral_xi_antibaryon):        return "neutral xi antibaryon";     break;
    case(negative_xi_baryon):           return "negative xi baryon";        break;
    case(positive_xi_antibaryon):       return "positive xi antibaryon";    break;
    case(negative_omega_baryon):        return "negative omega baryon";     break;
    case(positive_omega_antibaryon):    return "positive omega antibaryon"; break;
    case default: assert(false);
    }
    // clang-format on
}

// ================================================================================================

// TODO: std::string isn't constexpr until... when?
// TODO: std::string doesn't work on GPUs, right?  Is there a different data type I should use
//       instead?  Or should this format be explicitly excluded from running on a GPU?  What about
//       if someone wants to do some debugging of a GPU kernel and print out a tag?
//    -- That may depend on how much of the existing string manipulation framework I would have to
//       reimplement to make something that works on GPUs.
//    -- If I take out std::string, make sure to delete the include as well.
std::string to_short_standard_name(const Pantag tag)
{
    if (tag.is_nuclide()) {
        return to_short_iupac_name(tag);
    } else {
        return to_short_pdg_name(tag);
    }
}

// ================================================================================================

// TODO: std::string isn't constexpr until... when?
// TODO: std::string doesn't work on GPUs, right?  Is there a different data type I should use
//       instead?  Or should this format be explicitly excluded from running on a GPU?  What about
//       if someone wants to do some debugging of a GPU kernel and print out a tag?
//    -- That may depend on how much of the existing string manipulation framework I would have to
//       reimplement to make something that works on GPUs.
//    -- If I take out std::string, make sure to delete the include as well.
std::string to_long_standard_name(const Pantag tag)
{
    if (tag.is_nuclide()) {
        return to_long_iupac_name(tag);
    } else {
        return to_long_pdg_name(tag);
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
