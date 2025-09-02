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

// TODO: Currently elementals are coded as a special case of nuclides, and every standard tag is
//       either a nuclide or a particle.  Do I want to change that so that elementals are
//       considered a third type of entity separate from nuclides?  That could lose a bit of data
//       for particles, but that's probably not an issue (we currently have over 33 million valid
//       particle indices, so we'd still have almost 17 million valid particle indices if we lost a
//       bit).  It's a conceptual question: do we think of elements as special cases of nuclides or
//       as something else entirely?
//    -- We could actually handle this without losing a bit by treating the first two bits as
//       collectively defining a four-element code, for example:
//       -- 00: standard particle
//       -- 01: standard nuclide
//       -- 10: standard elemental
//       -- 11: user entity

// TODO: How do we handle ground states?
//    -- A non-elemental nuclide can be either in the ground state or in an excited state.  But
//       excited states have two different ways of indexing them:
//       -- An excited state index simply counts up all of the excited states and numbers them in
//          order from lowest energy to highest energy.  This is an excited state index.
//       -- A metastable state index defines some criteria for when an excited state is considerd a
//          metastable state, and indexes the same as excited states but skips any excited states
//          that don't meet the criteria of metastable states.  This is a metastable state index.
//    -- NDI only works in terms of metastable states, and so most LANL codes that I've encountered
//       appear to follow this convention (and implicitly use the same metastable criteria as NDI).
//       Wim points out that metastable states have problems and excited states are a better
//       choice, and so NJOY will (already does?) operate in terms of excited states.
//    -- It would be beyond the scope of Nautilus to implement transformations between excited and
//       metastable indices.  For one thing the metastable criteria can vary, but more importantly
//       that's a data question (usually involving lifetimes).
//    -- Currently Nautilus has the machinery to support both excited and metastable indices, but
//       without any ability to translate between the two.  It simply tracks what it's told and
//       reports that back.
//    -- If you're working only in excited states or only in metastable states, the ground state is
//       simply the lowest-energy state and therefore the state with index zero.  When you have
//       both excited and metastable states, the ground state is the only state where the excited
//       and metastable indices are guaranteed to coincide.
//    -- When creating a nuclide, we provide the option to specify an excited state index, specify
//       a metastable state index, or not specify an index at all.  Without an index, the nuclide
//       will default to the ground state (this is the reasonable choice for various reasons).
//    -- The question then arises: Is the ground state an excited state or a metastable state or
//       both?
//    -- And a related question: Is the default to encode the nuclide in an excited state or in a
//       metastable state?
//    -- The reason for this question is because of the separate accessors: get_metastable_index
//       and get_excitation_index, which only work if the nuclide in question is encoded to use
//       that type of index.
//    -- We could switch to some machinery where you can get the index and the type of index
//       separately.  For example, something like get_excitation_index() and is_metastable_state.
//       This cuts down how much we have to think about the special case of the ground state, but
//       does not eliminate it.
//    -- We could defer the question: Focus on the NDI use-case and only use metastable states.
//       That just means we'll have some incompatibility with NJOY (which reduces the utility of
//       Nautilus for someone who is interested in using it).  We could add excited states later to
//       address this.  But if we know excited states are likely to be useful, we might as well
//       work through the issues now so that we don't dig into a corner that we would have a hard
//       time backing out of later.
//    -- Assuming that we do include both excited and metastable indices, what questions would a
//       user likely want to ask?
//       -- For a code focused around NJOY, they want to ensure that the index is consistent with
//          excited state indexing.
//       -- For a code focused around NDI, they want to ensure that the index is consistent with
//          NDI indexing, then get the index.  We can't ensure that the input was provided
//          consistent with NDI's definition of metastable states, but can only report the index
//          that we were told.
//       -- The fact that metastable definitions vary is suggestive: Nautilus can't convert between
//          standards, so it only reports what it was told.  We _could_ potentially just skip the
//          entire question of _what kind of_ index we have, and just store the value of the index.
//          Then it's up to the user to be aware of which indexing convention they're using and do
//          any translations themselves.
//       -- The only failing to this is when converting to standard format.  A metastable state
//          will be reported with 'm', such as He-4m1, while an excited state will be reported with
//          'e', such as He-4e1.  The use of 'e' _may_ be unique to Wim, and I need to check on
//          this.
//       -- If you look at Wikipedia, there are also "fission isomers" (aka "shape isomers")
//          indicated with an 'f' (see especially the last paragraph of the "Metastable isomers"
//          section of Wikipedia's "Nuclear isomer" article:
//          https://en.wikipedia.org/wiki/Nuclear_isomer#Metastable_isomers).  Should Nautilus also
//          have the ability to support fission isomers?
//          -- Supporting fission isomers would require two bits to specify metastable, excitation,
//             or fission.  That would cut us down to only 7 bits for the index, and it's not clear
//             to me if that's sufficient.  I think Wim told me that there can be over a hundred
//             excited states for some isotopes.
//    -- One approach may be to ask Wim some follow-up questions:
//       -- Is the 'e' notation standard, or is 'm' standard regardless of whether you're using
//          excited vs metastable indices?
//       -- Is he aware of any usage of 'f' isomers?
//       -- What is the largest excitation index he would expect to run into?  The important point
//          here is: will it be > 127 (requiring 8 bits) or < 128 (fitting in 7 bits)?
//    -- If the only concern is for translating to other formats, then it may make sense to drop
//       the distinction except as an additional query: get_excitation_index and is_metastable or
//       something like that.
//       -- But this still doesn't account for the ground state.  When printing the ground state
//          won't use any annotation, although some formats append a 'g'.  Even if a 'g' is
//          necessary, this can be handled by checking is_ground.
//       -- I don't think it ever makes sense to allow the user to update the index without
//          specifying what kind of index it is.  So if we choose "wrong" by defaulting to (for
//          example) a "metastable" state with index 0, but someone wants an excitation index, we
//          don't really care because updating the index will require specifying that they're
//          giving an excitation index.  When querying (to convert to a new format), they'll be
//          informed that it's a ground state.
//       -- Alternately, if we add fission isomers, we could cheat a little:
//          -- There are three types of non-ground states: metastable ('m'), excitation ('e'), and
//             fission ('f').
//          -- Encoding this requires two bits, which actually gives us four options.
//          -- A default-constructed ground state can use the fourth combination to say ground
//             state ('g').
//          -- If a fourth type of excited state index is necessary, we can just repurpose that
//             fourth combination, because the ground state is still identifiable as having index
//             zero for every type of index.
//          -- Then when a new index is set, we could either go with what the user specifies or we
//             could check if this is a ground state and override it.
//          -- Or we could just arbitrarily pick one as a default, because functionally it would
//             probably make no difference observable to users.
//    -- A quick survey suggests that the vast majority of usages focus exclusively on metastable
//       states, with little reference to fission isomers and no discussion of all excitation
//       states except possible in things dealing with quantum mechanics where everything is done
//       in terms of other notations (such as quantum numbers).  If Wim is out on his own away from
//       anything standard, how much effort should Nautilus (read: Brendan) invest in supporting
//       his corner case?

// TODO: Check terminology.  Wikipedia seems to be saying that "isomer" specifically refers only to
//       metastable states, and not to all excited states.  But
//       https://en.wikipedia.org/wiki/Nuclide#Types_of_nuclides doesn't clearly address metastable
//       versus excitation, so it could be read as "all excited states" or "all metastable states".

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

    struct Unknown {};

    PORTABLE_FUNCTION constexpr auto unknown_tag() const
    {
        Storage unk_tag = 0;
        bs_user.set(USER, unk_tag);
        bs_data.set(UNKNOWN, unk_tag);
        bs_version.set(CURRENT_VERSION, unk_tag);
        return unk_tag;
    }

public:
    static constexpr Unknown unknown{};

    static constexpr User user{};

    static constexpr Storage elemental = 0b000000000;

    enum class Index { excitation, metastable };

    // ____________________________________________________________________________________________
    // Constructors

    PORTABLE_FUNCTION constexpr Pantag(const Unknown = unknown)
        : tag_{unknown_tag()}
    {
    }

    PORTABLE_FUNCTION constexpr Pantag(const Storage particle)
        : tag_{unknown_tag()}
    // TODO: Dummy argument to avoid compiler warnings -- does this indicate a problem?
    {
        set(particle);
    }
    PORTABLE_FUNCTION constexpr Pantag(const Storage Z, const Storage A)
        : tag_{unknown_tag()}
    // TODO: Dummy argument to avoid compiler warnings -- does this indicate a problem?
    {
        set(Z, A);
    }
    PORTABLE_FUNCTION constexpr Pantag(
        const Storage Z, const Storage A, const Index index, const Storage S)
        : tag_{unknown_tag()}
    // TODO: Dummy argument to avoid compiler warnings -- does this indicate a problem?
    {
        set(Z, A, index, S);
    }
    PORTABLE_FUNCTION constexpr Pantag(const User, const Storage data)
        : tag_{unknown_tag()}
    // TODO: Dummy argument to avoid compiler warnings -- does this indicate a problem?
    {
        set(user, data);
    }

    // TODO: Do I need explicit copy/move/assignment that checks the version and updates if there's
    //       a version mismatch?  Or do we assume that copy/move/assignment are already working
    //       with an updated version, and it's only other methods that have to handle old versions?

    // ____________________________________________________________________________________________
    // Build a Pantag

    PORTABLE_FUNCTION constexpr void set(const Unknown)
    {
        tag_ = unknown_tag();
    }
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
    PORTABLE_FUNCTION constexpr void set(
        const Storage Z, const Storage A, const Index index, const Storage S)
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

    PORTABLE_FUNCTION constexpr bool is_unknown() const { return tag_ == unknown_tag(); }

    PORTABLE_FUNCTION constexpr bool is_standard() const { return bs_user.get(tag_) == STANDARD; }
    PORTABLE_FUNCTION constexpr bool is_user() const {
        // an "unknown" pantag is encoded as a "user" tag with a special value
        if (is_unknown()) {
            return false;
        } else {
            return bs_user.get(tag_) == USER;
        }
    }

    PORTABLE_FUNCTION constexpr bool is_particle() const
    {
        assert(is_standard());
        return bs_nuclide.get(tag_) == PARTICLE;
    }
    PORTABLE_FUNCTION constexpr bool is_nuclide() const
    {
        assert(is_standard());
        return bs_nuclide.get(tag_) == NUCLIDE;
    }

    // Only for user tags.  For standard tags, the more-specific accessors are preferred, as there
    // may be translations between values the users sees and values actually stored in memory, or
    // the internal layout of the data block may be changed.
    // -- TODO: Does this imply I should use a different name?  Maybe get_user_data?
    PORTABLE_FUNCTION constexpr auto get_data() const
    {
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
    PORTABLE_FUNCTION constexpr bool is_ground() const
    {
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
