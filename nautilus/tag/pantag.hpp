#ifndef NAUTILUS_PANTAG_HPP
#define NAUTILUS_PANTAG_HPP

#include "bitsegment.hpp"

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
    //      NUDDDDDDDDDDDDDDDDDDDDDDDDDVVVVV
    //      |||                        |     rskip   bits   description
    //      |||                        \____  0       5     version
    //      ||\_____________________________  5      25     data
    //      |\______________________________ 30       1     user flag
    //      \_______________________________ 31       1     nuclide flag
    BitSegment<Storage, 31, 1> bs_nuclide;
    BitSegment<Storage, 30, 1> bs_user;
    BitSegment<Storage, 5, 25> bs_data;
    BitSegment<Storage, 0, 5> bs_version;

    // More detailed breakdown for standard nuclides
    //      10ZZZZZZZAAAAAAAAAMSSSSSSSSVVVVV
    //      |||      |        ||       |     rskip   bits   description
    //      |||      |        ||       \____  0       5     version
    //      |||      |        |\____________  5       8     excitation/metastable index
    //      |||      |        \_____________ 13       1     metastable flag
    //      |||      \______________________ 14       9     atomic mass number
    //      ||\_____________________________ 23       7     atomic number
    //      |\______________________________ 30       1     user flag (standard: 0)
    //      \_______________________________ 31       1     nuclide flag (nuclide: 1)
    BitSegment<Storage, 23, 7> bs_Z;
    BitSegment<Storage, 14, 9> bs_A;
    BitSegment<Storage, 13, 1> bs_exc_meta;
    BitSegment<Storage, 5, 8> bs_S;

    static constexpr Storage GROUND = 0b00000000;

    Storage tag_;

public:
    static constexpr Storage PARTICLE = 0b0;
    static constexpr Storage NUCLIDE = 0b1;

    static constexpr Storage STANDARD = 0b0;
    static constexpr Storage USER = 0b1;

    static constexpr Storage CURRENT_VERSION = 0b00000;

    static constexpr Storage EXCITATION_INDEX = 0b0;
    static constexpr Storage METASTABLE_INDEX = 0b1;

    PORTABLE_FUNCTION constexpr Pantag(Storage tag)
        : tag_{tag}
    {
        assert(bs_version.get(tag_) == CURRENT_VERSION);
    }

    // TODO: For purposes of type checking, it _may_ make sense to create enumerators for
    //       nuclide/particle, standard/user, excitation/metastable.  Then I would moved the named
    //       constants above back to being private.
    //    -- Using typed enums might also allow me to write a variation of this that defaults to
    //       standard: Pantag(const Enum nuclide_flag, const Storage data1, const Args... args)
    template <typename... Args>
    PORTABLE_FUNCTION constexpr Pantag(
        const Storage nuclide_flag, const Storage user_flag, const Args... args)
    {
        set(nuclide_flag, user_flag, args...);
    }

    // Build a Pantag

    PORTABLE_FUNCTION constexpr void set_standard_nuclide(const Storage Z, const Storage A)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_nuclide.set(NUCLIDE, tag_);
        bs_user.set(STANDARD, tag_);
        bs_Z.set(Z, tag_);
        bs_A.set(A, tag_);
        // TODO: Should we default to an excitation index or a metastable index?
        //    -- See notes below: maybe the ground state will return true for both index queries?
        bs_exc_meta.set(0b1, tag_); // TODO: ???
        bs_S.set(GROUND, tag_);
    }
    PORTABLE_FUNCTION constexpr void set_standard_nuclide(
        const Storage Z, const Storage A, const Storage state_type, const Storage S)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_nuclide.set(NUCLIDE, tag_);
        bs_user.set(STANDARD, tag_);
        bs_Z.set(Z, tag_);
        bs_A.set(A, tag_);
        bs_exc_meta.set(state_type, tag_);
        bs_S.set(S, tag_);
    }

    PORTABLE_FUNCTION constexpr void set_standard_particle(const Storage particle)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_nuclide.set(PARTICLE, tag_);
        bs_user.set(STANDARD, tag_);
        bs_data.set(particle, tag_);
    }

    PORTABLE_FUNCTION constexpr void set_user_nuclide(const Storage data)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_nuclide.set(NUCLIDE, tag_);
        bs_user.set(USER, tag_);
        bs_data.set(data, tag_);
    }

    PORTABLE_FUNCTION constexpr void set_user_particle(const Storage data)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_nuclide.set(PARTICLE, tag_);
        bs_user.set(USER, tag_);
        bs_data.set(data, tag_);
    }

    template <typename... Args>
    PORTABLE_FUNCTION constexpr void set(
        const Storage nuclide_flag, const Storage user_flag, const Args... args)
    {
        if (nuclide_flag == PARTICLE) {
            if (user_flag == STANDARD) {
                set_standard_particle(args...);
            } else {
                set_user_particle(args...);
            }
        } else {
            if (user_flag == STANDARD) {
                set_standard_nuclide(args...);
            } else {
                set_user_nuclide(args...);
            }
        }
    }

    // Generic accessors

    PORTABLE_FUNCTION constexpr bool is_particle() const
    {
        return bs_nuclide.get(tag_) == PARTICLE;
    }
    PORTABLE_FUNCTION constexpr bool is_nuclide() const { return bs_nuclide.get(tag_) == NUCLIDE; }

    PORTABLE_FUNCTION constexpr bool is_standard() const { return bs_user.get(tag_) == STANDARD; }
    PORTABLE_FUNCTION constexpr bool is_user() const { return bs_user.get(tag_) == USER; }

    PORTABLE_FUNCTION constexpr auto get_data() const { return bs_data.get(tag_); }

    PORTABLE_FUNCTION constexpr auto get_version() const { return bs_version.get(tag_); }

    // standard-Nuclide-specific accessors

    PORTABLE_FUNCTION constexpr auto get_atomic_number() const
    {
        assert(is_nuclide() && is_standard());
        return bs_Z.get(tag_);
    }
    PORTABLE_FUNCTION constexpr auto get_Z() const
    {
        assert(is_nuclide() && is_standard());
        return bs_Z.get(tag_);
    }

    PORTABLE_FUNCTION constexpr auto get_atomic_mass_number() const
    {
        assert(is_nuclide() && is_standard());
        return bs_A.get(tag_);
    }
    PORTABLE_FUNCTION constexpr auto get_A() const
    {
        assert(is_nuclide() && is_standard());
        return bs_A.get(tag_);
    }

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

    // Comparison operators

    PORTABLE_FUNCTION constexpr bool operator==(const Pantag other)
    {
        return tag_ == other.tag_;
    }
    PORTABLE_FUNCTION constexpr bool operator!=(const Pantag other)
    {
        return tag_ != other.tag_;
    }
    PORTABLE_FUNCTION constexpr bool operator<=(const Pantag other)
    {
        return tag_ <= other.tag_;
    }
    PORTABLE_FUNCTION constexpr bool operator>=(const Pantag other)
    {
        return tag_ >= other.tag_;
    }
    PORTABLE_FUNCTION constexpr bool operator<(const Pantag other)
    {
        return tag_ < other.tag_;
    }
    PORTABLE_FUNCTION constexpr bool operator>(const Pantag other)
    {
        return tag_ > other.tag_;
    }

};

// ================================================================================================

} // end namespace nautilus::tag

#endif // #ifndef NAUTILUS_PANTAG_HPP
