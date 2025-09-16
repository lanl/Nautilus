#ifndef NAUTILUS_ENTITYTAG_HPP
#define NAUTILUS_ENTITYTAG_HPP

#include "nautilus/entity_tag/bitsegment.hpp"
#include "nautilus/entity_tag/names.hpp"

#include "ports-of-call/portability.hpp"

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace nautilus::entity_tag {

// ================================================================================================

class EntityTag
{
private:
    using Storage = uint32_t;

    // Basic breakdown for all tags
    //      NDDDDDDDDDDDDDDDDDDDDDDDDDVVVVVV
    //      ||                        |      rskip   bits   description
    //      ||                        \_____  0       6     version
    //      |\______________________________  6      25     data
    //      \_______________________________ 31       1     user flag
    BitSegment<Storage, 31, 1> bs_user;
    BitSegment<Storage, 6, 25> bs_data;
    BitSegment<Storage, 0, 6> bs_version;

    // Breakdown for standard tags
    //      0NDDDDDDDDDDDDDDDDDDDDDDDDVVVVVV
    //      |||                       |      rskip   bits   description
    //      |||                       \_____  0       6     version
    //      ||\_____________________________  6      24     standard data
    //      |\______________________________ 30       1     nuclide flag
    //      \_______________________________ 31       1     user flag
    BitSegment<Storage, 30, 1> bs_nuclide;
    BitSegment<Storage, 6, 24> bs_sdata; // sdata = standard (non-user) tags

    static constexpr Storage PARTICLE = 0b0;
    static constexpr Storage NUCLIDE = 0b1;

    static constexpr Storage STANDARD = 0b0;
    static constexpr Storage USER = 0b1;

    // An "unknown" tag is encoded as a special value of "user" tag, but is not considered to
    // actually be a "user" tag itself
    static constexpr Storage UNKNOWN = 0b1111111111111111111111111;

    static constexpr Storage CURRENT_VERSION = 0b000000;

    // More detailed breakdown for standard nuclides
    //      01ZZZZZZZAAAAAAAAASSSSSSSSVVVVVV
    //      |||      |        |       |      rskip   bits   description
    //      |||      |        |       \_____  0       6     version
    //      |||      |        \_____________  6       8     excitation/metastable index
    //      |||      \______________________ 14       9     atomic mass number
    //      ||\_____________________________ 23       7     atomic number
    //      |\______________________________ 30       1     nuclide flag (nuclide: 1)
    //      \_______________________________ 31       1     user flag (standard: 0)
    BitSegment<Storage, 23, 7> bs_Z;
    BitSegment<Storage, 14, 9> bs_A;
    BitSegment<Storage, 6, 8> bs_S;

    static constexpr Storage elemental_A = 0b000000000;

    // More detailed breakdown for standard particles
    //      00IIIIIIIIIIIIIIIIIIIIIIIIVVVVVV
    //      |||                       |      rskip   bits   description
    //      |||                       \_____  0       6     version
    //      ||\_____________________________  6      24     particle index
    //      |\______________________________ 30       1     nuclide flag (particle: 0)
    //      \_______________________________ 31       1     user flag (standard: 0)
    // Note that the particle index is a genuine index (see the implementation in names.hpp), so
    // unless we add over 33 million more particles then we know the index is guaranteed to fit
    // within the particle index segment.
    BitSegment<Storage, 6, 24> bs_pindex;

    Storage tag_;

    struct User {};

    struct Unknown {};

    struct Elemental {};

    // return an EntityTag that's all zero to provide a known starting point
    PORTABLE_FUNCTION constexpr auto null_tag() const
    {
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

    static constexpr Elemental elemental{};

    // ____________________________________________________________________________________________
    // Constructors

    PORTABLE_FUNCTION constexpr EntityTag(const Unknown = unknown)
        : tag_{unknown_tag()}
    {}

    PORTABLE_FUNCTION constexpr explicit EntityTag(const Storage particle)
        : tag_{unknown_tag()}
    {
        set(particle);
    }
    PORTABLE_FUNCTION constexpr EntityTag(const Storage Z, const Storage A, const Storage S = 0)
        : tag_{unknown_tag()}
    {
        set(Z, A, S);
    }
    PORTABLE_FUNCTION constexpr EntityTag(const Storage Z, const Elemental)
        : tag_{unknown_tag()}
    {
        set(Z, elemental);
    }
    PORTABLE_FUNCTION constexpr EntityTag(const User, const Storage data)
        : tag_{unknown_tag()}
    {
        set(user, data);
    }

    // ____________________________________________________________________________________________
    // Build an EntityTag

    PORTABLE_FUNCTION constexpr void set(const Unknown) { tag_ = unknown_tag(); }
    PORTABLE_FUNCTION constexpr void set(const Storage particle)
    {
        tag_ = null_tag();
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(STANDARD, tag_);
        bs_nuclide.set(PARTICLE, tag_);
        bs_pindex.set(particle, tag_);
    }
    PORTABLE_FUNCTION constexpr void set(const Storage Z, const Storage A, const Storage S = 0)
    {
        tag_ = null_tag();
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(STANDARD, tag_);
        bs_nuclide.set(NUCLIDE, tag_);
        bs_Z.set(Z, tag_);
        bs_A.set(A, tag_);
        assert(A >= Z); // no negative neutron counts
        bs_S.set(S, tag_);
    }
    PORTABLE_FUNCTION constexpr void set(const Storage Z, const Elemental)
    {
        tag_ = null_tag();
        bs_version.set(CURRENT_VERSION, tag_);
        bs_user.set(STANDARD, tag_);
        bs_nuclide.set(NUCLIDE, tag_);
        bs_Z.set(Z, tag_);
        bs_A.set(elemental_A, tag_);
        bs_S.set(0, tag_); // S is meaningless with elementals, so set to ground state
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
        // an "unknown" tag is encoded as a "user" tag with a special value, but is not itself
        // considered a "user" tag
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

    PORTABLE_FUNCTION constexpr bool is_elemental() const { return get_A() == elemental_A; }

    PORTABLE_FUNCTION constexpr auto get_N() const { return get_A() - get_Z(); }
    PORTABLE_FUNCTION constexpr auto get_neutron_number() const { return get_N(); }

    // The metastable index doesn't apply to elementals, so it is assumed that elementals are in
    // the ground state.
    PORTABLE_FUNCTION constexpr auto get_metastable_index() const
    {
        assert(is_nuclide());
        return (is_elemental() ? 0 : bs_S.get(tag_));
    }
    PORTABLE_FUNCTION constexpr bool is_ground() const { return get_metastable_index() == 0; }

    // ____________________________________________________________________________________________
    // standard-particle-specific accessors

    PORTABLE_FUNCTION constexpr auto get_particle_index() const
    {
        assert(is_particle());
        return bs_pindex.get(tag_);
    }

    // ____________________________________________________________________________________________
    // Comparison operators

    PORTABLE_FUNCTION constexpr bool operator==(const EntityTag other)
    {
        return tag_ == other.tag_;
    }
    PORTABLE_FUNCTION constexpr bool operator!=(const EntityTag other)
    {
        return tag_ != other.tag_;
    }
    PORTABLE_FUNCTION constexpr bool operator<=(const EntityTag other)
    {
        return tag_ <= other.tag_;
    }
    PORTABLE_FUNCTION constexpr bool operator>=(const EntityTag other)
    {
        return tag_ >= other.tag_;
    }
    PORTABLE_FUNCTION constexpr bool operator<(const EntityTag other) { return tag_ < other.tag_; }
    PORTABLE_FUNCTION constexpr bool operator>(const EntityTag other) { return tag_ > other.tag_; }
};

// ================================================================================================

} // end namespace nautilus::entity_tag

#endif // #ifndef NAUTILUS_ENTITYTAG_HPP
