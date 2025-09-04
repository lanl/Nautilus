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
//    -- EntityTag?  I don't want to go all the way to something super-generic like "tag" or
//       "label" or "identifier" in case we introduce some other kind of tag/label/identifier (for
//       example, something like Pantag but for reaction identifiers).  I can't think of a good
//       name that includes particles, nuclides, elementals, and similar.  Perhaps "entity" or
//       something similar is the best we can do, relying on the fact that it's the EntityTag _from
//       Nautilus_ to provide sufficient context regarding what it is.

// TODO: Add a discussion to the documentation about excited vs metastable states
//    -- Metastable is not well-defined
//    -- All formats currently supported (appear to) share the same metastable definition
//    -- Some formats we haven't (yet?) implemented may use different definition of metastable
//    -- Some formats we haven't (yet?) implemented, such as GNDS and upcoming "modern" NJOY use
//       excitation state indices.
//    -- Interestingly, https://en.wikipedia.org/wiki/Nuclear_isomer also gives two definitions of
//       metastable and the reasons for them, giving a nice illustration of why "metastable" is
//       ill-defined:
//       > "Metastable" describes nuclei whose excited states have half-lives of 1e−9 seconds or
//       > longer, 100 to 1000 times longer than the half-lives of the excited nuclear states that
//       > decay with a "prompt" half-life (ordinarily on the order of 1e−12 seconds). Some
//       > references recommend 5e−9 seconds to distinguish the metastable half-life from the
//       > normal "prompt" gamma-emission half-life.
//       The citation Wikipedia gives for this is  Walker, Philip M.; Carroll, James J. (2007).
//       "Nuclear Isomers: Recipes from the Past and Ingredients for the Future" (PDF). Nuclear
//       Physics News. 17 (2): 11–15. doi:10.1080/10506890701404206. S2CID 22342780.

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

    // An "unknown" tag is a special value of "user" tags
    static constexpr Storage UNKNOWN = 0b11111111111111111111111111;

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
    // TODO: See if I can shift bs_S to fill the empty bit, then shorten the standard particle and
    //       user ranges to extend the version range.
    BitSegment<Storage, 5, 8> bs_S;

    static constexpr Storage EXCITATION_INDEX = 0b0;
    static constexpr Storage METASTABLE_INDEX = 0b1;

    static constexpr Storage GROUND = 0b00000000;

    // More detailed breakdown for standard particles
    //      00IIIIIIIIIIIIIIIIIIIIIIIIIVVVVV
    //      |||                        |     rskip   bits   description
    //      |||                        \____  0       5     version
    //      ||\_____________________________ 11      25     particle index
    //      |\______________________________ 30       1     nuclide flag (particle: 0)
    //      \_______________________________ 31       1     user flag (standard: 0)
    // Note that the particle index is a genuine index (see the implementation in names.hpp), so
    // unless we add over 33 million more particles then we know the index is guaranteed to fit
    // within the particle index segment.
    BitSegment<Storage, 5, 6> bs_pindex;

    Storage tag_;

    struct User {};

    struct Unknown {};

    PORTABLE_FUNCTION constexpr auto null_tag() const
    {
        // return a tag that's all zero to provide a known starting point
        Storage null_tag = 0;
        null_tag = null_tag & ~null_tag;
        return null_tag;
    }

    PORTABLE_FUNCTION constexpr auto unknown_tag() const
    {
        Storage unk_tag = null_tag();
        bs_user.set(USER, unk_tag);
        bs_data.set(UNKNOWN, unk_tag);
        bs_version.set(CURRENT_VERSION, unk_tag);
        return unk_tag;
    }

public:
    static constexpr Unknown unknown{};

    static constexpr User user{};

    // TODO: Should this be a struct like unknown and user?
    static constexpr Storage elemental = 0b000000000;

    // ____________________________________________________________________________________________
    // Constructors

    PORTABLE_FUNCTION constexpr Pantag(const Unknown = unknown)
        : tag_{unknown_tag()}
    {}

    PORTABLE_FUNCTION constexpr Pantag(const Storage particle)
        : tag_{unknown_tag()}
    {
        set(particle);
    }
    PORTABLE_FUNCTION constexpr Pantag(const Storage Z, const Storage A, const Storage S=GROUND)
        : tag_{unknown_tag()}
    {
        set(Z, A, S);
    }
    PORTABLE_FUNCTION constexpr Pantag(const User, const Storage data)
        : tag_{unknown_tag()}
    {
        set(user, data);
    }

    // TODO: Do I need explicit copy/move/assignment that checks the version and updates if there's
    //       a version mismatch?  Or do we assume that copy/move/assignment are already working
    //       with an updated version, and it's only other methods that have to handle old versions?

    // ____________________________________________________________________________________________
    // Build a Pantag

    PORTABLE_FUNCTION constexpr void set(const Unknown) { tag_ = unknown_tag(); }
    PORTABLE_FUNCTION constexpr void set(const Storage particle)
    {
        tag_ = null_tag();
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(STANDARD, tag_);
        bs_nuclide.set(PARTICLE, tag_);
        bs_pindex.set(particle, tag_);
    }
    PORTABLE_FUNCTION constexpr void set(const Storage Z, const Storage A, const Storage S=GROUND)
    {
        tag_ = null_tag();
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(STANDARD, tag_);
        bs_nuclide.set(NUCLIDE, tag_);
        bs_Z.set(Z, tag_);
        bs_A.set(A, tag_);
        assert(A == elemental ? S == GROUND : true); // S is meaningless with elementals
        bs_S.set(S, tag_);
    }
    PORTABLE_FUNCTION constexpr void set(const User, const Storage data)
    {
        tag_ = null_tag();
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(USER, tag_);
        bs_data.set(data, tag_);
    }

    // ____________________________________________________________________________________________
    // Generic accessors

    PORTABLE_FUNCTION static constexpr auto current_version() { return CURRENT_VERSION; }
    PORTABLE_FUNCTION constexpr auto get_version() const { return bs_version.get(tag_); }

    // The user interface exposes a three-state system: standard, user, or unknown, which are
    // mutually exclusive and cover the full range of possible tags.
    PORTABLE_FUNCTION constexpr bool is_unknown() const { return tag_ == unknown_tag(); }
    PORTABLE_FUNCTION constexpr bool is_standard() const { return bs_user.get(tag_) == STANDARD; }
    PORTABLE_FUNCTION constexpr bool is_user() const
    {
        // an "unknown" pantag is encoded as a "user" tag with a special value
        if (is_unknown()) {
            return false;
        } else {
            return bs_user.get(tag_) == USER;
        }
    }

    // subsets of standard tags
    PORTABLE_FUNCTION constexpr bool is_particle() const
    {
        return (is_standard() ? bs_nuclide.get(tag_) == PARTICLE : false);
    }
    PORTABLE_FUNCTION constexpr bool is_nuclide() const
    {
        return (is_standard() ? bs_nuclide.get(tag_) == NUCLIDE : false);
    }

    // ____________________________________________________________________________________________
    // user-specific accessors

    // Only for user tags.  For standard tags, the more-specific accessors are preferred, as there
    // may be translations between values the users sees and values actually stored in memory, or
    // the internal layout of the data block may be changed.
    PORTABLE_FUNCTION constexpr auto get_user_data() const
    {
        assert(is_user());
        return bs_data.get(tag_);
    }

    // ____________________________________________________________________________________________
    // standard-nuclide-specific accessors

    PORTABLE_FUNCTION constexpr auto get_Z() const
    {
        assert(is_nuclide());
        return bs_Z.get(tag_);
    }
    PORTABLE_FUNCTION constexpr auto get_atomic_number() const { return get_Z(); }

    PORTABLE_FUNCTION constexpr auto get_A() const
    {
        assert(is_nuclide());
        assert(!is_elemental());
        return bs_A.get(tag_);
    }
    PORTABLE_FUNCTION constexpr auto get_atomic_mass_number() const { return get_A(); }

    PORTABLE_FUNCTION constexpr bool is_elemental() const
    {
        return get_A() == elemental;
    }

    PORTABLE_FUNCTION constexpr auto get_N() const { return get_A() - get_Z(); }
    PORTABLE_FUNCTION constexpr auto get_neutron_number() const { return get_N(); }

    // TODO: Is the name "get_metastable_index" still appropriate, or should I change it?
    //    -- Related: Should I add shorthand like get_Z and get_A?  NDI would want get_S while MCNP
    //       would want get_m, so maybe both or maybe neither?
    // The metastable index doesn't apply to elementals, so it is assumed that elementals are in
    // the ground state.
    PORTABLE_FUNCTION constexpr auto get_metastable_index() const
    {
        assert(is_nuclide());
        return (is_elemental() ? GROUND : bs_S.get(tag_));
    }
    PORTABLE_FUNCTION constexpr bool is_ground() const
    {
        return get_metastable_index() == GROUND;
    }

    // ____________________________________________________________________________________________
    // standard-particle-specific accessors

    PORTABLE_FUNCTION constexpr auto get_particle_index() const
    {
        assert(is_particle());
        return bs_pindex.get(tag_);
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
