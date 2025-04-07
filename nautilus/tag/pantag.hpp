#ifndef NAUTILUS_PANTAG_HPP
#define NAUTILUS_PANTAG_HPP

#include "nautilus/tag/bitsegment.hpp"
#include "nautilus/tag/names.hpp"

#include "ports-of-call/portability.hpp"

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace nautilus::tag {

// ================================================================================================

// TODO: Pantag doesn't strike me as a great name, but it'll do as a placeholder for now

// Particle-and-Nuclide Tag
class Pantag
{
private:
    using Storage = uint32_t;

    // Basic breakdown for all tags
    //      NDDDDDDDDDDDDDDDDDDDDDDDDDDVVVVV
    //      ||                         |     rskip   bits   description
    //      ||                         \____  0       5     version
    //      |\______________________________  5      26     data
    //      \_______________________________ 31       1     user flag
    BitSegment<Storage, 31, 1> bs_user;
    BitSegment<Storage, 5, 26> bs_data;
    BitSegment<Storage, 0, 5> bs_version;

    // Breakdown for standard tags
    //      0NDDDDDDDDDDDDDDDDDDDDDDDDDVVVVV
    //      |||                        |     rskip   bits   description
    //      |||                        \____  0       5     version
    //      ||\_____________________________  5      25     standard data
    //      |\______________________________ 30       1     nuclide flag
    //      \_______________________________ 31       1     user flag
    BitSegment<Storage, 30, 1> bs_nuclide;
    BitSegment<Storage, 5, 25> bs_sdata; // sdata = standard (non-user) tags

    static constexpr Storage PARTICLE = 0b0;
    static constexpr Storage NUCLIDE = 0b1;

    static constexpr Storage STANDARD = 0b0;
    static constexpr Storage USER = 0b1;

    static constexpr Storage CURRENT_VERSION = 0b00000;

    // More detailed breakdown for standard nuclides
    //      01ZZZZZZZAAAAAAAAAMSSSSSSSSVVVVV
    //      |||      |        ||       |     rskip   bits   description
    //      |||      |        ||       \____  0       5     version
    //      |||      |        |\____________  5       8     excitation/metastable index
    //      |||      |        \_____________ 13       1     metastable flag
    //      |||      \______________________ 14       9     atomic mass number
    //      ||\_____________________________ 23       7     atomic number
    //      |\______________________________ 30       1     nuclide flag (nuclide: 1)
    //      \_______________________________ 31       1     user flag (standard: 0)
    BitSegment<Storage, 23, 7> bs_Z;
    BitSegment<Storage, 14, 9> bs_A;
    BitSegment<Storage, 13, 1> bs_exc_meta;
    BitSegment<Storage, 5, 8> bs_S;

    static constexpr Storage EXCITATION_INDEX = 0b0;
    static constexpr Storage METASTABLE_INDEX = 0b1;

    static constexpr Storage GROUND = 0b00000000;

    // More detailed breakdown for standard particles
    //      000000000000000000000HCIIIAVVVVV
    //      |||                  |||  ||     rskip   bits   description
    //      |||                  |||  |\____  0       5     version
    //      |||                  |||  \_____  5       1     antiparticle flag
    //      |||                  ||\________  6       3     particle index
    //      |||                  |\_________  9       1     category flag
    //      |||                  \__________ 10       1     hadron flag
    //      ||\_____________________________ 11      19     (unused, set to all zero)
    //      |\______________________________ 30       1     nuclide flag (particle: 0)
    //      \_______________________________ 31       1     user flag (standard: 0)
    BitSegment<Storage, 10, 1> bs_hadron;
    BitSegment<Storage, 9, 1> bs_category;
    BitSegment<Storage, 6, 3> bs_pindex;
    BitSegment<Storage, 5, 1> bs_anti;

    Storage tag_;

    // TODO: These codes were chosen to make it easier to answer certain questions about particles
    //       (for example: is this a hadron?  is this a second-generation lepton?).  I took out
    //       those accessors for now.  If those accessors are not re-added, is the additional
    //       compilcation of translating between particle indices and particle codes worth
    //       maintaining?
    PORTABLE_FUNCTION static constexpr Storage index_to_code(const std::size_t index)
    {
        // clang-format off
        switch (index) {
        case names::photon:                    return 0b000000; break;
        case names::electron:                  return 0b010000; break;
        case names::positron:                  return 0b010001; break;
        case names::electron_neutrino:         return 0b010010; break;
        case names::electron_antineutrino:     return 0b010011; break;
        case names::muon:                      return 0b010100; break;
        case names::antimuon:                  return 0b010101; break;
        case names::muon_neutrino:             return 0b010110; break;
        case names::muon_antineutrino:         return 0b010111; break;
        case names::neutral_pion:              return 0b100000; break;
        case names::positive_pion:             return 0b100010; break;
        case names::negative_pion:             return 0b100011; break;
        case names::short_kaon:                return 0b100100; break;
        case names::long_kaon:                 return 0b100110; break;
        case names::positive_kaon:             return 0b101000; break;
        case names::negative_kaon:             return 0b101001; break;
        case names::neutron:                   return 0b110000; break;
        case names::antineutron:               return 0b110001; break;
        case names::proton:                    return 0b110010; break;
        case names::antiproton:                return 0b110011; break;
        case names::neutral_lambda_baryon:     return 0b110100; break;
        case names::neutral_lambda_antibaryon: return 0b110101; break;
        case names::positive_sigma_baryon:     return 0b110110; break;
        case names::negative_sigma_antibaryon: return 0b110111; break;
        case names::negative_sigma_baryon:     return 0b111000; break;
        case names::positive_sigma_antibaryon: return 0b111001; break;
        case names::neutral_xi_baryon:         return 0b111010; break;
        case names::neutral_xi_antibaryon:     return 0b111011; break;
        case names::negative_xi_baryon:        return 0b111100; break;
        case names::positive_xi_antibaryon:    return 0b111101; break;
        case names::negative_omega_baryon:     return 0b111110; break;
        case names::positive_omega_antibaryon: return 0b111111; break;
        default:
            assert(false);
            return 0b000001; // Doesn't match any particles above
        };
        // clang-format on
    }
    PORTABLE_FUNCTION static constexpr std::size_t code_to_index(Storage code)
    {
        using namespace names;
        // clang-format off
        switch (code) {
        case index_to_code(photon):                    return photon; break;
        case index_to_code(electron):                  return electron; break;
        case index_to_code(positron):                  return positron; break;
        case index_to_code(electron_neutrino):         return electron_neutrino; break;
        case index_to_code(electron_antineutrino):     return electron_antineutrino; break;
        case index_to_code(muon):                      return muon; break;
        case index_to_code(antimuon):                  return antimuon; break;
        case index_to_code(muon_neutrino):             return muon_neutrino; break;
        case index_to_code(muon_antineutrino):         return muon_antineutrino; break;
        case index_to_code(neutral_pion):              return neutral_pion; break;
        case index_to_code(positive_pion):             return positive_pion; break;
        case index_to_code(negative_pion):             return negative_pion; break;
        case index_to_code(short_kaon):                return short_kaon; break;
        case index_to_code(long_kaon):                 return long_kaon; break;
        case index_to_code(positive_kaon):             return positive_kaon; break;
        case index_to_code(negative_kaon):             return negative_kaon; break;
        case index_to_code(neutron):                   return neutron; break;
        case index_to_code(antineutron):               return antineutron; break;
        case index_to_code(proton):                    return proton; break;
        case index_to_code(antiproton):                return antiproton; break;
        case index_to_code(neutral_lambda_baryon):     return neutral_lambda_baryon; break;
        case index_to_code(neutral_lambda_antibaryon): return neutral_lambda_antibaryon; break;
        case index_to_code(positive_sigma_baryon):     return positive_sigma_baryon; break;
        case index_to_code(negative_sigma_antibaryon): return negative_sigma_antibaryon; break;
        case index_to_code(negative_sigma_baryon):     return negative_sigma_baryon; break;
        case index_to_code(positive_sigma_antibaryon): return positive_sigma_antibaryon; break;
        case index_to_code(neutral_xi_baryon):         return neutral_xi_baryon; break;
        case index_to_code(neutral_xi_antibaryon):     return neutral_xi_antibaryon; break;
        case index_to_code(negative_xi_baryon):        return negative_xi_baryon; break;
        case index_to_code(positive_xi_antibaryon):    return positive_xi_antibaryon; break;
        case index_to_code(negative_omega_baryon):     return negative_omega_baryon; break;
        case index_to_code(positive_omega_antibaryon): return positive_omega_antibaryon; break;
        default:
            assert(false);
            return names::Particles::count;
        };
        // clang-format off
    }

    struct User {};

public:
    // TODO: Do we need this anymore?
    enum class PNType { particle, nuclide };

    // TODO: Do we need this anymore?
    enum class Mode { standard, user };

    static constexpr User user;

    static constexpr Storage elemental = 0b000000000;

    enum class Index { excitation, metastable };

    // ____________________________________________________________________________________________
    // Constructors

    // TODO: I need to think about the right interface(s) to construct and modify a Pantag.
    //    -- What I currently have:
    //       -- constructors and set use the same argument lists (constructors just defer to set)
    //       -- set(nuclide, standard, Z, A, index_type, S)
    //       -- set(nuclide, Z, A, index_type, S)
    //       -- set(nuclide, standard, Z, A)
    //       -- set(nuclide, Z, A)
    //       -- set(nuclide, user, data)
    //       -- set(particle, standard, pindex)
    //       -- set(particle, pindex)
    //       -- set(particle, user, data)
    //    -- defaults
    //       -- Is it useful to allow users to default to "standard"?
    //       -- Is it useful to allow users to default to ground state for nuclides?
    //       -- Getting rid of both default options means users would always have to use the
    //          most-verbose form of set(nuclide, standard, Z, A, index_type, S) to build a
    //          standard nuclide.
    //    -- How often will users directly construct a Pantag vs construct one of the other formats
    //       and the convert that to a Pantag?
    //       -- Initial usage is expected to focus on Pantag as a vehicle for translating between
    //          formats.  In that case, users will build something in format A, then write a
    //          conversion routine that converts from format A to Pantag to format B, then returns
    //          format B.  Pantag will only exist as a temporary between the conversions and users
    //          won't actually ever construct a Pantag directly.
    //       -- If users adopt Pantag as their internal format (e.g., if xRAGE were to replace
    //          NDI's zaid with Pantag), then the use case would look something more like
    //          -- Read in user input.  User input comes in a text format, so there's some
    //             translation routine that will need to be used.  This might be something like
    //             "from_standard_name" (or another format that Nautilus already implements).
    //          -- Add in any non-user-specified isotopes, such as some kind of standard reaction
    //             chain, or commonly-used nuclides, etc.  These could be constructed directly as a
    //             Pantag (which would be the logical choice if Pantag were the internal format for
    //             the code) or it could be a conversion from a zaid or other format such as
    //                my_tag = from_ndi_zaid(zaid(Z, A, S))
    //             This would be a slightly odd workflow to build in a different format just to
    //             immediately convert to Pantag.  That would suggest that Pantag's
    //             constructors/setters aren't up to the task and users had to work around them to
    //             get what they wanted by using another format as a shortcut.
    //    -- Given the current concept for Pantag, where things are split between nuclides and
    //       particles, particles are identified by an index, and nuclides are identified by Z, A,
    //       and S, there may not be much improvement to be had to the general concept of the
    //       interface (to say nothing of the particular implementation).
    //    -- What if we drop the particle/nuclide type and the mode entirely, and have three
    //       constructors:
    //       -- (Z, A): nuclide
    //       -- (Z, A, i, S): nuclide
    //       -- (index): particle
    //       Particles have no multi-argument constructors, because you simply identify them by the
    //       index.  Nuclides have no single-argument constructors, because at the very least you
    //       need to specify the Z and A value.  There are also elementals, but you still need
    //       multiple arguments: Z and an elemental flag (implementation-wise, that will probably
    //       just run through the (Z,A) constructor).  Because of this split, there's no need to
    //       specify that you have a nuclide or a particle, because it's trivial to distinguish
    //       between them.  That would also simplify the interface by eliminating the need to
    //       explicitly tag as a nuclide or particle.
    //    -- But under this new idea, how would a user tag work?  Presumably, there would be a
    //       named constant (possibly a tag type?) that would be the first argument, and then a
    //       data argument that simply gets packed into the tag.

    PORTABLE_FUNCTION constexpr Pantag(const Storage particle)
    : tag_{0} // TODO: Dummy argument to avoid compiler warnings -- does this indicate a problem?
    {
        set(particle);
    }
    PORTABLE_FUNCTION constexpr Pantag(const Storage Z, const Storage A)
    : tag_{0} // TODO: Dummy argument to avoid compiler warnings -- does this indicate a problem?
    {
        set(Z, A);
    }
    PORTABLE_FUNCTION constexpr Pantag(const Storage Z, const Storage A, const Index index, const Storage S)
    : tag_{0} // TODO: Dummy argument to avoid compiler warnings -- does this indicate a problem?
    {
        set(Z, A, index, S);
    }
    PORTABLE_FUNCTION constexpr Pantag(const User, const Storage data)
    : tag_{0} // TODO: Dummy argument to avoid compiler warnings -- does this indicate a problem?
    {
        set(user, data);
    }

    // TODO: Do I need explicit copy/move/assignment that checks the version and updates if there's
    //       a version mismatch?  Or do we assume that copy/move/assignment are already working
    //       with an updated version, and it's only other methods that have to handle old versions?

    // ____________________________________________________________________________________________
    // Build a Pantag

    PORTABLE_FUNCTION constexpr void set(const Storage particle)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(STANDARD, tag_);
        bs_nuclide.set(PARTICLE, tag_);
        // We set bs_sdata because:
        // (a) this will zero out the unused section, and
        // (b) this will set all of the particle data bits at once, instead of having to set each
        //     individually.
        bs_sdata.set(index_to_code(particle), tag_);
    }
    PORTABLE_FUNCTION constexpr void set(const Storage Z, const Storage A)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(STANDARD, tag_);
        bs_nuclide.set(NUCLIDE, tag_);
        bs_Z.set(Z, tag_);
        bs_A.set(A, tag_);
        // TODO: Should we default to an excitation index or a metastable index?
        //    -- See notes below: maybe the ground state will return true for both index queries?
        //    -- Or maybe having a default on the index is just a bad idea?
        //    -- Another option: Maybe we don't add excitation states at all and just stick with
        //       metastable states?  Wim is planning to use excitation states in NJOY, but it's not
        //       clear if any current codes use excitation states.  Adding excitation states could
        //       be a future addition.
        bs_exc_meta.set(METASTABLE_INDEX, tag_);
        bs_S.set(GROUND, tag_);
    }
    PORTABLE_FUNCTION constexpr void set(const Storage Z, const Storage A, const Index index, const Storage S)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(STANDARD, tag_);
        bs_nuclide.set(NUCLIDE, tag_);
        bs_Z.set(Z, tag_);
        bs_A.set(A, tag_);
        assert(A == elemental ? S == 0 : true); // elementals don't have excited/metastable state
        switch (index) {
        case Index::excitation: bs_exc_meta.set(EXCITATION_INDEX, tag_); break;
        case Index::metastable: bs_exc_meta.set(METASTABLE_INDEX, tag_); break;
        }
        bs_S.set(S, tag_);
    }
    PORTABLE_FUNCTION constexpr void set(const User, const Storage data)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(USER, tag_);
        bs_data.set(data, tag_);
    }

    // ____________________________________________________________________________________________
    // Generic accessors

    PORTABLE_FUNCTION static constexpr auto version() { return CURRENT_VERSION; }

    PORTABLE_FUNCTION constexpr bool is_standard() const { return bs_user.get(tag_) == STANDARD; }
    PORTABLE_FUNCTION constexpr bool is_user() const { return bs_user.get(tag_) == USER; }

    PORTABLE_FUNCTION constexpr bool is_particle() const
    {
        assert(is_standard());
        return bs_nuclide.get(tag_) == PARTICLE;
    }
    PORTABLE_FUNCTION constexpr bool is_nuclide() const {
        assert(is_standard());
        return bs_nuclide.get(tag_) == NUCLIDE;
    }

    // Primarily intended for user tags.  For standard tags, the more-specific accessors are
    // preferred, as there may be translations between values the users sees and values actually
    // stored in memory, or the internal layout of the data block may be changed.
    // TODO: I can ensure this is only used for user tags with an assertion, but there may be uses
    //       in test code, and possible in production code.  Should I add an assertion?
    PORTABLE_FUNCTION constexpr auto get_data() const { return bs_data.get(tag_); }

    PORTABLE_FUNCTION constexpr auto get_version() const { return bs_version.get(tag_); }

    // ____________________________________________________________________________________________
    // standard-nuclide-specific accessors

    PORTABLE_FUNCTION constexpr auto get_Z() const
    {
        // TODO: The nuclide-specific and particle-specific accessors currently check that the tag
        //       is both a (particle|nuclide) _and_ that it is standard.  That is still the right
        //       set of checks to make except it should change in one way or another:
        //       (a) is_nuclide and is_particle now check is_standard.  If this is sufficient, then
        //           the assertion in these accessors should just be is_nuclide or is_particle.
        //       (b) If that check is not sufficient, then these assertions should be reversed
        //           because only standard tags can meaningfully answer the is_nuclide /
        //           is_particle question.
        assert(is_nuclide() && is_standard());
        return bs_Z.get(tag_);
    }
    PORTABLE_FUNCTION constexpr auto get_atomic_number() const { return get_Z(); }

    PORTABLE_FUNCTION constexpr bool is_elemental() const
    {
        assert(is_nuclide() && is_standard());
        return bs_A.get(tag_) == elemental;
    }

    PORTABLE_FUNCTION constexpr auto get_A() const
    {
        assert(is_nuclide() && is_standard());
        return bs_A.get(tag_);
    }
    PORTABLE_FUNCTION constexpr auto get_atomic_mass_number() const { return get_A(); }

    PORTABLE_FUNCTION constexpr auto get_N() const { return get_A() - get_Z(); }
    PORTABLE_FUNCTION constexpr auto get_neutron_number() const { return get_N(); }

    // TODO: The ground state has the same index (0) regardless of whether you are using excitation
    //       or metastable indices.  Should an index of zero be considered to have both an
    //       excitation index AND a metastable index (i.e., both of these return true)?  If not,
    //       then constructing a nuclide and defaulting to the ground state requires that Nautilus
    //       chooses either an excitation index or a metastable index.
    //    -- Alternately: version 0 only has a metastable index, and we only add excitation states
    //       later as needed.
    PORTABLE_FUNCTION constexpr auto has_excitation_index() const
    {
        assert(is_nuclide() && is_standard());
        return (bs_exc_meta.get(tag_) == EXCITATION_INDEX) || (bs_S.get(tag_) == GROUND);
    }
    PORTABLE_FUNCTION constexpr auto has_metastable_index() const
    {
        assert(is_nuclide() && is_standard());
        return (bs_exc_meta.get(tag_) == METASTABLE_INDEX) || (bs_S.get(tag_) == GROUND);
    }

    PORTABLE_FUNCTION constexpr auto get_excitation_index() const
    {
        assert(is_nuclide() && is_standard() && has_excitation_index());
        return bs_S.get(tag_);
    }
    PORTABLE_FUNCTION constexpr auto get_metastable_index() const
    {
        assert(is_nuclide() && is_standard() && has_metastable_index());
        return bs_S.get(tag_);
    }
    PORTABLE_FUNCTION constexpr bool is_ground() const {
        assert(is_nuclide() && is_standard());
        return bs_S.get(tag_) == GROUND;
    }
    // TODO: get_index doesn't seem like a good name
    //    -- maybe `get_energy_level_index`?
    PORTABLE_FUNCTION constexpr auto get_index() const
    {
        assert(is_nuclide() && is_standard());
        return bs_S.get(tag_);
    }

    // ____________________________________________________________________________________________
    // standard-particle-specific accessors

    PORTABLE_FUNCTION constexpr auto get_particle_index() const
    {
        assert(is_particle() && is_standard());
        return code_to_index(bs_pindex.get(tag_));
    }

    // ____________________________________________________________________________________________
    // Comparison operators

    PORTABLE_FUNCTION constexpr bool operator==(const Pantag other) { return tag_ == other.tag_; }
    PORTABLE_FUNCTION constexpr bool operator!=(const Pantag other) { return tag_ != other.tag_; }
    PORTABLE_FUNCTION constexpr bool operator<=(const Pantag other) { return tag_ <= other.tag_; }
    PORTABLE_FUNCTION constexpr bool operator>=(const Pantag other) { return tag_ >= other.tag_; }
    PORTABLE_FUNCTION constexpr bool operator<(const Pantag other) { return tag_ < other.tag_; }
    PORTABLE_FUNCTION constexpr bool operator>(const Pantag other) { return tag_ > other.tag_; }
};

// ================================================================================================

} // end namespace nautilus::tag

#endif // #ifndef NAUTILUS_PANTAG_HPP
