#ifndef NAUTILUS_SZA_HPP
#define NAUTILUS_SZA_HPP

#include "ports-of-call/portability.hpp"

#include <string>
#include <type_traits>

namespace nautilus {

// SZA representation of an isotope
// S = metastable state number
//    (note: this is not the same as the excitation level as not all
//    excitation levels will be long-lived enough to be classified as
//    metastable states.)
// Z = atomic number
// A = mass number
class SZA
{
public:
    // single-argument constructor is explicit
    PORTABLE_FUNCTION constexpr explicit SZA(int sza)
        : sza_(sza)
    {}

    // get full SZA
    PORTABLE_FUNCTION constexpr SZA(int Z, int A, int S = 0)
        : sza_{S * s_shift + Z * z_shift + A}
    {}

    // get S component of SZA
    PORTABLE_FUNCTION constexpr int S() const { return sza_ / s_shift; }

    // get Z component of SZA
    PORTABLE_FUNCTION constexpr int Z() const { return (sza_ % s_shift) / z_shift; }

    // get A component of SZA
    PORTABLE_FUNCTION constexpr int A() const { return sza_ % z_shift; }

    // get full SZA
    PORTABLE_FUNCTION constexpr explicit operator int() const { return sza_; }

    // equality comparison
    PORTABLE_FUNCTION constexpr bool operator==(const SZA & other) const
    {
        return sza_ == other.sza_;
    }

    // inequality comparison
    PORTABLE_FUNCTION constexpr bool operator!=(const SZA & other) const
    {
        return sza_ != other.sza_;
    }

    // inequality comparison
    PORTABLE_FUNCTION constexpr bool operator<(const SZA & other) const
    {
        auto this_za = sza_ % s_shift;
        auto other_za = other.sza_ % s_shift;
        if (this_za == other_za) {
            auto this_s = S();
            auto other_s = other.S();
            return this_s < other_s;
        } else {
            return this_za < other_za;
        }
    }

    // inequality comparison
    PORTABLE_FUNCTION constexpr bool operator>=(const SZA & other) const
    {
        return !(*this < other);
    }

    // inequality comparison
    PORTABLE_FUNCTION constexpr bool operator<=(const SZA & other) const
    {
        auto this_za = sza_ % s_shift;
        auto other_za = other.sza_ % s_shift;
        if (this_za == other_za) {
            auto this_s = S();
            auto other_s = other.S();
            return this_s <= other_s;
        } else {
            return this_za <= other_za;
        }
    }

    // inequality comparison
    PORTABLE_FUNCTION constexpr bool operator>(const SZA & other) const
    {
        return !(*this <= other);
    }

private:
    static constexpr int s_shift{1000000};
    static constexpr int z_shift{1000};
    int sza_;
};

} // namespace nautilus

#endif // NAUTILUS_SZA_HPP
