// TODO: Should this be in the tag/ subdirectory?

#include <array>
#include <string_view>

#include "ports-of-call/portability.hpp"

// ================================================================================================

namespace nautilus::names {

// ================================================================================================

template <std::size_t N>
class Identifiers
{
private:
    std::string_view symbol_;
    std::array<std::string_view, N> names_;

#define ENUMERATION(E) typename E, std::enable_if_t<std::is_enum<E>::value>* = nullptr

    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr void set(
        const std::string_view alternate_name, const E alt_standard)
    {
        const std::size_t alt_index = static_cast<std::size_t>(alt_standard);
        assert(alt_index < names_.size());
        names_[alt_index] = alternate_name;
    }
    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr std::string_view get(const E alt_standard) const
    {
        const std::size_t alt_index = static_cast<std::size_t>(alt_standard);
        assert(alt_index < names_.size());
        return names_[alt_index];
    }

public:
    // Constructor
    PORTABLE_FUNCTION constexpr Identifiers(
        const std::string_view symbol, const std::string_view standard_name)
        : symbol_{symbol}
    {
        for (auto & name : names_) {
            name = standard_name;
        }
    }
    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr Identifiers(
        const std::string_view symbol,
        const std::string_view standard_name,
        const std::string_view alternate_name,
        const E alt_standard)
        : Identifiers(symbol, standard_name)
    {
        set(alternate_name, alt_standard);
    }
    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr Identifiers(
        const std::string_view symbol,
        const std::string_view standard_name,
        const std::string_view alternate_name,
        const E alt_standard1,
        const E alt_standard2)
        : Identifiers(symbol, standard_name)
    {
        set(alternate_name, alt_standard1);
        set(alternate_name, alt_standard2);
    }

    // Fetch
    PORTABLE_FUNCTION constexpr std::string_view get_symbol() const { return symbol_; }
    // It's assumed that E(0) will be the default value.
    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr std::string_view get_name(const E standard = E(0)) const
    {
        return get(standard);
    }

    // Match
    PORTABLE_FUNCTION constexpr bool match_symbol(const std::string_view symbol) const
    {
        return symbol == symbol_;
    }
    PORTABLE_FUNCTION constexpr bool match_name(const std::string_view name) const
    {
        for (const auto & name0 : names_) {
            if (name0 == name) {
                return true;
            }
        }
        return false;
    }
};

// ================================================================================================

class Nuclides {
private:
    using Nuclide = Identifiers<4>;
public:
    static constexpr std::size_t count = 118;
    // It's assumed that Standard(0) will be the default value.
    enum class Standard : std::size_t { IUPAC, American, British, Canadian };
private:
    // This structure was chosen because global constexpr variables are not, in general, available
    // on the GPU.  By turning this into a private method and combining the accessor methods into
    // the class, we end up with a GPU-compatible structure.
    PORTABLE_FUNCTION static constexpr auto get_identifiers(const std::size_t Z)
    {
        // clang-format off
        constexpr std::array<Nuclide, count> nuclides{
            Nuclide("H" , "hydrogen"    ), // Z = 1, index = 0
            Nuclide("He", "helium"      ),
            Nuclide("Li", "lithium"     ),
            Nuclide("Be", "beryllium"   ),
            Nuclide("B" , "boron"       ),
            Nuclide("C" , "carbon"      ),
            Nuclide("N" , "nitrogen"    ),
            Nuclide("O" , "oxygen"      ),
            Nuclide("F" , "fluorine"    ),
            Nuclide("Ne", "neon"        ),
            Nuclide("Na", "sodium"      ), // Z = 11, index = 10
            Nuclide("Mg", "magnesium"   ),
            Nuclide("Al", "aluminium",  "aluminum", Standard::American, Standard::Canadian),
            Nuclide("Si", "silicon"     ),
            Nuclide("P" , "phosphorus"  ),
            Nuclide("S" , "sulfur",     "sulphur",  Standard::British),
            Nuclide("Cl", "chlorine"    ),
            Nuclide("Ar", "argon"       ),
            Nuclide("K" , "potassium"   ),
            Nuclide("Ca", "calcium"     ),
            Nuclide("Sc", "scandium"    ), // Z = 21, index = 20
            Nuclide("Ti", "titanium"    ),
            Nuclide("V" , "vanadium"    ),
            Nuclide("Cr", "chromium"    ),
            Nuclide("Mn", "manganese"   ),
            Nuclide("Fe", "iron"        ),
            Nuclide("Co", "cobalt"      ),
            Nuclide("Ni", "nickel"      ),
            Nuclide("Cu", "copper"      ),
            Nuclide("Zn", "zinc"        ),
            Nuclide("Ga", "gallium"     ), // Z = 31, index = 30
            Nuclide("Ge", "germanium"   ),
            Nuclide("As", "arsenic"     ),
            Nuclide("Se", "selenium"    ),
            Nuclide("Br", "bromine"     ),
            Nuclide("Kr", "krypton"     ),
            Nuclide("Rb", "rubidium"    ),
            Nuclide("Sr", "strontium"   ),
            Nuclide("Y" , "yttrium"     ),
            Nuclide("Zr", "zirconium"   ),
            Nuclide("Nb", "niobium"     ), // Z = 41, index = 40
            Nuclide("Mo", "molybdenum"  ),
            Nuclide("Tc", "technetium"  ),
            Nuclide("Ru", "ruthenium"   ),
            Nuclide("Rh", "rhodium"     ),
            Nuclide("Pd", "palladium"   ),
            Nuclide("Ag", "silver"      ),
            Nuclide("Cd", "cadmium"     ),
            Nuclide("In", "indium"      ),
            Nuclide("Sn", "tin"         ),
            Nuclide("Sb", "antimony"    ), // Z = 51, index = 50
            Nuclide("Te", "tellurium"   ),
            Nuclide("I" , "iodine"      ),
            Nuclide("Xe", "xenon"       ),
            Nuclide("Cs", "caesium",    "cesium",   Standard::American),
            Nuclide("Ba", "barium"      ),
            Nuclide("La", "lanthanum"   ),
            Nuclide("Ce", "cerium"      ),
            Nuclide("Pr", "praseodymium"),
            Nuclide("Nd", "neodymium"   ),
            Nuclide("Pm", "promethium"  ), // Z = 61, index = 60
            Nuclide("Sm", "samarium"    ),
            Nuclide("Eu", "europium"    ),
            Nuclide("Gd", "gadolinium"  ),
            Nuclide("Tb", "terbium"     ),
            Nuclide("Dy", "dysprosium"  ),
            Nuclide("Ho", "holmium"     ),
            Nuclide("Er", "erbium"      ),
            Nuclide("Tm", "thulium"     ),
            Nuclide("Yb", "ytterbium"   ),
            Nuclide("Lu", "lutetium"    ), // Z = 71, index = 70
            Nuclide("Hf", "hafnium"     ),
            Nuclide("Ta", "tantalum"    ),
            Nuclide("W" , "tungsten"    ),
            Nuclide("Re", "rhenium"     ),
            Nuclide("Os", "osmium"      ),
            Nuclide("Ir", "iridium"     ),
            Nuclide("Pt", "platinum"    ),
            Nuclide("Au", "gold"        ),
            Nuclide("Hg", "mercury"     ),
            Nuclide("Tl", "thallium"    ), // Z = 81, index = 80
            Nuclide("Pb", "lead"        ),
            Nuclide("Bi", "bismuth"     ),
            Nuclide("Po", "polonium"    ),
            Nuclide("At", "astatine"    ),
            Nuclide("Rn", "radon"       ),
            Nuclide("Fr", "francium"    ),
            Nuclide("Ra", "radium"      ),
            Nuclide("Ac", "actinium"    ),
            Nuclide("Th", "thorium"     ),
            Nuclide("Pa", "protactinium"), // Z = 91, index = 90
            Nuclide("U" , "uranium"     ),
            Nuclide("Np", "neptunium"   ),
            Nuclide("Pu", "plutonium"   ),
            Nuclide("Am", "americium"   ),
            Nuclide("Cm", "curium"      ),
            Nuclide("Bk", "berkelium"   ),
            Nuclide("Cf", "californium" ),
            Nuclide("Es", "einsteinium" ),
            Nuclide("Fm", "fermium"     ),
            Nuclide("Md", "mendelevium" ), // Z = 101, index = 100
            Nuclide("No", "nobelium"    ),
            Nuclide("Lr", "lawrencium"  ),
            Nuclide("Rf", "rutherforium"),
            Nuclide("Db", "dubnium"     ),
            Nuclide("Sg", "seaborgium"  ),
            Nuclide("Bh", "bohrium"     ),
            Nuclide("Hs", "hassium"     ),
            Nuclide("Mt", "meitnerium"  ),
            Nuclide("Ds", "darmstadtium"),
            Nuclide("Rg", "roentgenium" ), // Z = 111, index = 110
            Nuclide("Cn", "copernicium" ),
            Nuclide("Nh", "nihonium"    ),
            Nuclide("Fl", "flerovium"   ),
            Nuclide("Mc", "moscovium"   ),
            Nuclide("Lv", "livermorium" ),
            Nuclide("Ts", "tennessine"  ),
            Nuclide("Og", "oganesson"   ),
        };
        // clang-format on
        assert(Z > 0);
        assert(Z <= nuclides.size());
        return nuclides[Z-1];
    }

public:
    PORTABLE_FUNCTION static constexpr std::size_t get_index(const std::string_view query)
    {
        for (std::size_t Z = 1; Z <= count; ++Z) {
            const auto ids = get_identifiers(Z);
            if (ids.match_symbol(query) || ids.match_name(query)) {
                return Z;
            }
        }
        assert(false);
        return count + 1;
    }
    PORTABLE_FUNCTION static constexpr std::string_view get_symbol(const std::size_t Z)
    {
        return get_identifiers(Z).get_symbol();
    }

    PORTABLE_FUNCTION static constexpr std::string_view get_name(
        const std::size_t Z, const Standard standard=Standard::IUPAC)
    {
        return get_identifiers(Z).get_name(standard);
    }
};

// ================================================================================================

struct Particles {
private:
    using Particle = Identifiers<2>;
public:
    static constexpr std::size_t count = 32;
    // It's assumed that Standard(0) will be the default value.
    // TODO: I don't really care for the name "alternate", because it's not descriptive of what
    //       this format actually is.
    enum class Standard : std::size_t { PDG, alternate };
private:
    // This structure was chosen because global constexpr variables are not, in general, available
    // on the GPU.  By turning this into a private method and combining the accessor methods into
    // the class, we end up with a GPU-compatible structure.
    PORTABLE_FUNCTION static constexpr auto get_identifiers(const std::size_t index)
    {
        // Warning: The symbols use UTF-8 code points, so the string may not behave as expected
        //          (for example, size may report a longer result than the actual number of printed
        //          characters).  Using something like wstring instead leads to different
        //          complications, so this was the compromise chosen by Nautilus.
        // Warning: Subscript mu does not appear to exist in Unicode at this time, so I had to
        //          substitute a subscript lowercase m.
        // Warning: Subscript uppercase L and S don't appear to exist in Unicode at this time, and
        //          they would be more standard for the long and short kaons.  Instead I had to
        //          substitute lowercase subscripts.
        // Warning: Unicode does not provide the ability to stack a subscript and a superscript
        //          vertically, but must place them side-by-side.  For the long and short kaons, we
        //          place the superscript first and the subscript after.
        // clang-format off
        constexpr std::array<Particle, count> particles{
            Particle("\u03B3",              "photon"),
            Particle("e\u207B",             "electron"),
            Particle("e\u207A",             "positron"),
            Particle("\u03BD\u2091",        "electron neutrino"),
            Particle("\u03BD\u0304\u2091",  "electron antineutrino"),
            Particle("\u03Bc\u207B",        "muon"),
            Particle("\u03BC\u0304\u207A",  "antimuon"),
            Particle("\u03BD\u2098",        "muon neutrino"),
            Particle("\u03BD\u0304\u2098",   "muon antineutrino"),
            Particle("\u03C0\u2070",        "neutral pion"),
            Particle("\u03C0\u207A",        "positive pion"),
            Particle("\u03C0\u207B",        "negative pion"),
            Particle("K\u2070\u209B",       "short kaon"),
            Particle("K\u2070\u2097",       "long kaon"),
            Particle("K\u207A",             "positive kaon"),
            Particle("K\u207B",             "negative kaon"),
            Particle("n",                   "neutron"),
            Particle("n\u0304",             "antineutron"),
            Particle("p",                   "proton"),
            Particle("p\u0304",             "antiproton"),
            Particle("\u039B\u2070",        "neutral lambda baryon"),
            Particle("\u039B\u0304\u2070",  "neutral lambda antibaryon",
                    "antiparticle of the neutral lambda baryon",    Standard::alternate),
            Particle("\u03A3\u207A",        "positive sigma baryon"),
            Particle("\u03A3\u0304\u207B",  "negative sigma antibaryon",
                    "antiparticle of the positive sigma baryon",    Standard::alternate),
            Particle("\u03A3\u207B",        "negative sigma baryon"),
            Particle("\u03A3\u0304\u207A",  "positive sigma antibaryon",
                    "antiparticle of the negative sigma baryon",    Standard::alternate),
            Particle("\u039E\u2070",        "neutral xi baryon"),
            Particle("\u039E\u0304\u2070",  "neutral xi antibaryon",
                    "antiparticle of the neutral xi baryon",        Standard::alternate),
            Particle("\u039E\u207B",        "negative xi baryon"),
            Particle("\u039E\u0304\u207A",  "positive xi antibaryon",
                    "antiparticle of the negative xi baryon",       Standard::alternate),
            Particle("\u03A9\u207B",        "negative omega baryon"),
            Particle("\u03A9\u0304\u207A",  "positive omega antibaryon",
                    "antiparticle of the negative omega baryon",    Standard::alternate),
        };
        // clang-format on
        assert(index < particles.size());
        return particles[index];
    }

public:
    PORTABLE_FUNCTION static constexpr std::size_t get_index(const std::string_view query)
    {
        for (std::size_t index = 0; index < count; ++index) {
            const auto ids = get_identifiers(index);
            if (ids.match_symbol(query) || ids.match_name(query)) {
                return index;
            }
        }
        assert(false);
        return count;
    }
    // The "alternate" convention cannot be represented in Unicode, so there is no flag to select
    // different versions of the symbol.  You always get the PDG format.
    PORTABLE_FUNCTION static constexpr std::string_view get_symbol(const std::size_t index)
    {
        return get_identifiers(index).get_symbol();
    }

    PORTABLE_FUNCTION static constexpr std::string_view get_name(
        const std::size_t index, const Standard standard=Standard::PDG)
    {
        return get_identifiers(index).get_name(standard);
    }
};

// ================================================================================================

// The values of these indices are meaningless.  It is simply the order they are found within the
// above list, and that order may change at any point without being considered a break in
// compatibility.  This just gives names we can type to reference the particles.  The only
// guarantee is that the names are contiguous and start at zero, as they are the indices within a
// std::array.
#define PARTICLE_INDEX(var, str) \
    static constexpr std::size_t var = Particles::get_index(str);
PARTICLE_INDEX(photon, "photon");
PARTICLE_INDEX(electron, "electron");
PARTICLE_INDEX(positron, "positron");
PARTICLE_INDEX(electron_neutrino, "electron neutrino");
PARTICLE_INDEX(electron_antineutrino, "electron antineutrino");
PARTICLE_INDEX(muon, "muon");
PARTICLE_INDEX(antimuon, "antimuon");
PARTICLE_INDEX(muon_neutrino, "muon neutrino");
PARTICLE_INDEX(muon_antineutrino, "muon antineutrino");
PARTICLE_INDEX(neutral_pion, "neutral pion");
PARTICLE_INDEX(positive_pion, "positive pion");
PARTICLE_INDEX(negative_pion, "negative pion");
PARTICLE_INDEX(short_kaon, "short kaon");
PARTICLE_INDEX(long_kaon, "long kaon");
PARTICLE_INDEX(positive_kaon, "positive kaon");
PARTICLE_INDEX(negative_kaon, "negative kaon");
PARTICLE_INDEX(neutron, "neutron");
PARTICLE_INDEX(antineutron, "antineutron");
PARTICLE_INDEX(proton, "proton");
PARTICLE_INDEX(antiproton, "antiproton");
PARTICLE_INDEX(neutral_lambda_baryon, "neutral lambda baryon");
PARTICLE_INDEX(neutral_lambda_antibaryon, "neutral lambda antibaryon");
PARTICLE_INDEX(positive_sigma_baryon, "positive sigma baryon");
PARTICLE_INDEX(negative_sigma_antibaryon, "negative sigma antibaryon");
PARTICLE_INDEX(negative_sigma_baryon, "negative sigma baryon");
PARTICLE_INDEX(positive_sigma_antibaryon, "positive sigma antibaryon");
PARTICLE_INDEX(neutral_xi_baryon, "neutral xi baryon");
PARTICLE_INDEX(neutral_xi_antibaryon, "neutral xi antibaryon");
PARTICLE_INDEX(negative_xi_baryon, "negative xi baryon");
PARTICLE_INDEX(positive_xi_antibaryon, "positive xi antibaryon");
PARTICLE_INDEX(negative_omega_baryon, "negative omega baryon");
PARTICLE_INDEX(positive_omega_antibaryon, "positive omega antibaryon");

// ================================================================================================

} // namespace nautilus::names

// ================================================================================================
