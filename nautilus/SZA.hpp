#ifndef NAUTILUS_SZA_HPP
#define NAUTILUS_SZA_HPP

#include "ports-of-call/portability.hpp"

#include <string>
#include <type_traits>

namespace nautilus {

// SZA representation of a nuclide
class SZA
{
public:
    // constructors
    PORTABLE_FUNCTION constexpr explicit SZA(int sza)
        : sza_(sza)
    {}
    PORTABLE_FUNCTION constexpr SZA(int Z, int A, int S = 0)
        : sza_{S * s_shift + Z * z_shift + A}
    {}

    // accessors
    PORTABLE_FUNCTION constexpr int S() const { return sza_ / s_shift; }
    PORTABLE_FUNCTION constexpr int Z() const { return (sza_ % s_shift) / z_shift; }
    PORTABLE_FUNCTION constexpr int A() const { return sza_ % z_shift; }
    PORTABLE_FUNCTION constexpr int N() const { return A() - Z(); }
    PORTABLE_FUNCTION constexpr explicit operator int() const { return sza_; }

    // comparisons
    PORTABLE_FUNCTION constexpr bool operator==(const SZA & other) const
    {
        return sza_ == other.sza_;
    }
    PORTABLE_FUNCTION constexpr bool operator!=(const SZA & other) const
    {
        return sza_ != other.sza_;
    }
    PORTABLE_FUNCTION constexpr bool operator<(const SZA & other) const
    {
        auto this_za = sza_ % s_shift;
        auto other_za = other.sza_ % s_shift;
        return (this_za == other_za ? S() < other.S() : this_za < other_za);
    }
    PORTABLE_FUNCTION constexpr bool operator>=(const SZA & other) const
    {
        return !(*this < other);
    }
    PORTABLE_FUNCTION constexpr bool operator<=(const SZA & other) const
    {
        auto this_za = sza_ % s_shift;
        auto other_za = other.sza_ % s_shift;
        return (this_za == other_za ? S() <= other.S() : this_za <= other_za);
    }
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
