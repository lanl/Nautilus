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

// TODO: I have a bunch of accessors/queries defined below for standard nuclides and standard
//       particles, and I also thought about but rejected others.  I think it's not really clear
//       what the "right" set of methods is, particularly for particles.
//    -- With nuclides, you'll use the queries to translate to other formats.  For example, use the
//       atomic number to look up the chemical symbol, add in the atomic mass number, and
//       potentially append a qualification with the metastable state.
//    -- With particles, you can't really use these to uniquely identify the particle.  Instead,
//       you pretty much have to do some kind of if-else ladder.  If we only want the right methods
//       to identify things, then we just need the enum and all other particle-specific methods
//       should go away.  But maybe it's useful to ask things like, "Is this a hadron?" or, "Are
//       these two particles a particle-antiparticle pair?" 
//    -- I'm not even convinced that having the different segments of the standard particle format
//       broken out into BigSegments is worth having.
//    -- Note, I already deleted the are_antiparticles because the ambiguity around truly-neutral
//       particles makes it hard to define that method until you do a better job of formulating the
//       question that the method is answering.
//    -- A similar ambiguity exists in whether the ground state is an excited state, a metastable
//       state, or both.  I need to more clearly formulate the question that the method is
//       answering, otherwise I don't really know what the answer should be in that case.
//    -- I think I need to stop writing accessors, and instead back up and start working on the
//       use-cases, starting with the translation routines.  Instead of saying, "Here is a question
//       that someone _could_ ask, so I'll _try_ to implement the answer," I should be saying,
//       "Here is a use-case where a question has come up, so I should write the method that
//       provides the answer."
//    -- While the particle queries don't uniquely identify the particle, it's possible that they
//       may have some value in categorizing the particles in a way that makes the _average_
//       runtime of the method shorter.  That is, for example, knowing you have a lepton means you
//       only have to check a subset of the full list of particles.  But this would require
//       checking the implementation details to see which (if any) methods can improve query time.
//    -- I should start by implementing a detailed format that covers everything (even if it's one
//       of the verbose "pretty-printing" formats) just so that I can see how the logic works out
//       in as complete an example as possible.

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

    // TODO: There may be some value in having these defined as they are.  But it may make sense to
    //       instead use bs_nuclide.mask() (and similar below for standard/user) and compare
    //       against a calibrated value that removes the need to shift and then compare.  Instead,
    //       make it a mask-and-compare.  But that couples things (the values here would have to be
    //       kept consistent with the BigSegment definitions above), and there may be other reasons
    //       why keeping things like this makes sense.
    static constexpr Storage PARTICLE = 0b0;
    static constexpr Storage NUCLIDE = 0b1;

    static constexpr Storage STANDARD = 0b0;
    static constexpr Storage USER = 0b1;

    static constexpr Storage CURRENT_VERSION = 0b00000;

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

    static constexpr Storage EXCITATION_INDEX = 0b0;
    static constexpr Storage METASTABLE_INDEX = 0b1;

    static constexpr Storage GROUND = 0b00000000;

    // TODO: Write tests for particles and their accessors.

    // More detailed breakdown for standard particles
    //      00???????????????????HCIIIAVVVVV
    //      |||                  |||  ||     rskip   bits   description
    //      |||                  |||  |\____  0       5     version
    //      |||                  |||  \_____  5       1     antiparticle flag
    //      |||                  ||\________  6       3     particle index
    //      |||                  |\_________  9       1     category flag
    //      |||                  \__________ 10       1     hadron flag
    //      ||\_____________________________ 11      19     (unused)
    //      |\______________________________ 30       1     user flag (standard: 0)
    //      \_______________________________ 31       1     nuclide flag (nuclide: 1)
    BitSegment<Storage, 10, 1> bs_hadron;
    BitSegment<Storage, 9, 1> bs_category;
    BitSegment<Storage, 6, 3> bs_pindex;
    BitSegment<Storage, 5, 1> bs_anti;
    
    static constexpr Storage ELEMENTARY = 0b1;
    static constexpr Storage COMPOSITE = 0b1;

    static constexpr Storage BOSON = 0b0;
    static constexpr Storage MESON = 0b0;
    static constexpr Storage LEPTON = 0b1;
    static constexpr Storage BARYON = 0b1;

    Storage tag_;

public:
    enum class PNType { particle, nuclide };

    enum class Mode { standard, user };

    static constexpr Storage elemental = 0b000000000;

    enum class Index { excitation, metastable };

    enum class Particles {
        // elementary bosons
        photon =                    0b000000,
        // elementary leptons
        electron =                  0b010000,
        positron =                  0b010001,
        electron_neutrino =         0b010010,
        electron_antineutrino =     0b010011,
        muon =                      0b010100,
        antimuon =                  0b010101,
        muon_neutrino =             0b010110,
        muon_antineutrino =         0b010111,
        // composite leptons
        neutral_pion =              0b100000,
        positive_pion =             0b100010,
        negative_pion =             0b100011,
        short_kaon =                0b100100,
        long_kaon =                 0b100110,
        positive_kaon =             0b101000,
        negative_kaon =             0b101001,
        // composite baryons
        neutron =                   0b110000,
        antineutron =               0b110001,
        proton =                    0b110010,
        antiproton =                0b110011,
        neutral_lambda_baryon =     0b110100,
        neutral_lambda_antibaryon = 0b110100,
        positive_sigma_baryon =     0b110110,
        negative_sigma_antibaryon = 0b110111,
        negative_sigma_baryon =     0b111000,
        positive_sigma_antibaryon = 0b111001,
        neutral_xi_baryon =         0b111010,
        neutral_xi_antibaryon =     0b111011,
        negative_xi_baryon =        0b111100,
        positive_xi_antibaryon =    0b111101,
        negative_omega_baryon =     0b111110,
        positive_omega_antibaryon = 0b111111,
        // alternate baryon antiparticle convention
        anti_positive_sigma_baryon = negative_sigma_antibaryon,
        anti_negative_sigma_baryon = positive_sigma_antibaryon,
        anti_negative_xi_baryon    = positive_xi_antibaryon,
        anti_negative_omega_baryon = positive_omega_antibaryon,
    };

    PORTABLE_FUNCTION constexpr Pantag(Storage tag)
        : tag_{tag}
    {
        assert(bs_version.get(tag_) == CURRENT_VERSION);
    }

    template <typename... Args>
    PORTABLE_FUNCTION constexpr Pantag(const PNType pntype, const Mode mode, const Args... args)
    {
        set(pntype, mode, args...);
    }
    template <typename... Args>
    PORTABLE_FUNCTION constexpr Pantag(const PNType pntype, const Storage s0, const Args... args)
    {
        set(pntype, Mode::standard, s0, args...);
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
        bs_exc_meta.set(METASTABLE_INDEX, tag_); // TODO: ???
        bs_S.set(GROUND, tag_);
    }
    PORTABLE_FUNCTION constexpr void set_standard_nuclide(
        const Storage Z, const Storage A, const Index index, const Storage S)
    {
        bs_version.set(CURRENT_VERSION, tag_);
        bs_nuclide.set(NUCLIDE, tag_);
        bs_user.set(STANDARD, tag_);
        bs_Z.set(Z, tag_);
        bs_A.set(A, tag_);
        switch (index) {
        case Index::excitation: bs_exc_meta.set(EXCITATION_INDEX, tag_); break;
        case Index::metastable: bs_exc_meta.set(METASTABLE_INDEX, tag_); break;
        }
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
    PORTABLE_FUNCTION constexpr void set(const PNType pntype, const Mode mode, const Args... args)
    {
        switch (pntype) {
        case PNType::particle:
            switch (mode) {
            case Mode::standard: // set_standard_particle(args...); break;
                // TODO: This works, but it's so brittle that I'm not comfortable with it.
                if constexpr (sizeof...(args) == 1) {
                    set_standard_particle(args...);
                } else {
                    assert(false);
                }
                break;
            case Mode::user: // set_user_particle(args...); break;
                if constexpr (sizeof...(args) == 1) {
                    set_user_particle(args...);
                } else {
                    assert(false);
                }
                break;
            }
            break;
        case PNType::nuclide:
            switch (mode) {
            case Mode::standard: // set_standard_nuclide(args...); break;
                if constexpr ((sizeof...(args) >= 2) && (sizeof...(args) <= 4)) {
                    set_standard_nuclide(args...);
                } else {
                    assert(false);
                }
                break;
            case Mode::user: // set_user_nuclide(args...); break;
                if constexpr (sizeof...(args) == 1) {
                    set_user_nuclide(args...); break;
                } else {
                    assert(false);
                }
                break;
            }
            break;
        }
    }
    template <typename... Args>
    PORTABLE_FUNCTION constexpr void set(const PNType pntype, const Storage s0, const Args... args)
    {
        set(pntype, Mode::standard, s0, args...);
    }

    // Generic accessors

    PORTABLE_FUNCTION static constexpr auto version() { return CURRENT_VERSION; }

    PORTABLE_FUNCTION constexpr bool is_particle() const
    {
        return bs_nuclide.get(tag_) == PARTICLE;
    }
    PORTABLE_FUNCTION constexpr bool is_nuclide() const { return bs_nuclide.get(tag_) == NUCLIDE; }

    PORTABLE_FUNCTION constexpr bool is_standard() const { return bs_user.get(tag_) == STANDARD; }
    PORTABLE_FUNCTION constexpr bool is_user() const { return bs_user.get(tag_) == USER; }

    PORTABLE_FUNCTION constexpr auto get_data() const { return bs_data.get(tag_); }

    PORTABLE_FUNCTION constexpr auto get_version() const { return bs_version.get(tag_); }

    // standard-nuclide-specific accessors

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

    // standard-nuclide-specific accessors

    PORTABLE_FUNCTION constexpr bool is_elementary() const
    {
        assert(is_particle() && is_standard());
        return bs_hadron.get(tag_) == ELEMENTARY;
    }
    PORTABLE_FUNCTION constexpr bool is_composite() const
    {
        assert(is_particle() && is_standard());
        return bs_hadron.get(tag_) == COMPOSITE;
    }
    PORTABLE_FUNCTION constexpr bool is_hadron() const
    {
        assert(is_particle() && is_standard());
        return bs_hadron.get(tag_) == COMPOSITE;
    }

    PORTABLE_FUNCTION constexpr bool is_boson() const
    {
        assert(is_particle() && is_standard());
        return is_elementary() && (bs_category.get(tag_) == BOSON);
    }
    PORTABLE_FUNCTION constexpr bool is_meson() const
    {
        assert(is_particle() && is_standard());
        return is_composite() && (bs_category.get(tag_) == MESON);
    }
    PORTABLE_FUNCTION constexpr bool is_lepton() const
    {
        assert(is_particle() && is_standard());
        return is_elementary() && (bs_category.get(tag_) == LEPTON);
    }
    PORTABLE_FUNCTION constexpr bool is_baryon() const
    {
        assert(is_particle() && is_standard());
        return is_composite() && (bs_category.get(tag_) == BARYON);
    }

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
