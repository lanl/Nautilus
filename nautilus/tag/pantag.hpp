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

public:
    static constexpr User user;

    static constexpr Storage elemental = 0b000000000;

    enum class Index { excitation, metastable };

    // ____________________________________________________________________________________________
    // Constructors

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
        bs_pindex.set(particle, tag_);
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

    // Only for user tags.  For standard tags, the more-specific accessors are preferred, as there
    // may be translations between values the users sees and values actually stored in memory, or
    // the internal layout of the data block may be changed.
    PORTABLE_FUNCTION constexpr auto get_data() const {
        assert(is_user());
        return bs_data.get(tag_);
    }

    PORTABLE_FUNCTION constexpr auto get_version() const { return bs_version.get(tag_); }

    // ____________________________________________________________________________________________
    // standard-nuclide-specific accessors

    PORTABLE_FUNCTION constexpr auto get_Z() const
    {
        assert(is_nuclide());
        return bs_Z.get(tag_);
    }
    PORTABLE_FUNCTION constexpr auto get_atomic_number() const { return get_Z(); }

    PORTABLE_FUNCTION constexpr bool is_elemental() const
    {
        assert(is_nuclide());
        return bs_A.get(tag_) == elemental;
    }

    PORTABLE_FUNCTION constexpr auto get_A() const
    {
        assert(is_nuclide());
        assert(!is_elemental());
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
        assert(is_nuclide());
        if (is_elemental()) {
            return false;
        } else if (bs_S.get(tag_) == GROUND) {
            return true;
        } else {
            return bs_exc_meta.get(tag_) == EXCITATION_INDEX;
        }
    }
    PORTABLE_FUNCTION constexpr auto has_metastable_index() const
    {
        assert(is_nuclide());
        if (is_elemental()) {
            return false;
        } else if (bs_S.get(tag_) == GROUND) {
            return true;
        } else {
            return bs_exc_meta.get(tag_) == METASTABLE_INDEX;
        }
    }

    PORTABLE_FUNCTION constexpr auto get_excitation_index() const
    {
        assert(is_nuclide() && has_excitation_index());
        return bs_S.get(tag_);
    }
    PORTABLE_FUNCTION constexpr auto get_metastable_index() const
    {
        assert(is_nuclide() && has_metastable_index());
        return bs_S.get(tag_);
    }
    PORTABLE_FUNCTION constexpr bool is_ground() const {
        assert(is_nuclide());
        assert(!is_elemental());
        return bs_S.get(tag_) == GROUND;
    }
    // TODO: get_index doesn't seem like a good name
    //    -- maybe `get_energy_level_index`?
    PORTABLE_FUNCTION constexpr auto get_index() const
    {
        assert(is_nuclide());
        assert(!is_elemental());
        return bs_S.get(tag_);
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
