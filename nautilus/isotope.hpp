#ifndef NAUTILUS_ISOTOPE_HPP
#define NAUTILUS_ISOTOPE_HPP

#include "nautilus/SZA.hpp"

#include "ports-of-call/portability.hpp"
#include "ports-of-call/robust_utils.hpp"

#include <cassert>
#include <iostream>

namespace nautilus {

// ================================================================================================

// Type for specifying isotopes.
template <typename DataType>
struct Isotope {
private:
    // The SZA value
    /*
    Handles S, Z, and A collectively and communicates with things that prefer the unified SZA
    concept.
    */
    SZA sza_;

    // The mass of the isotope
    DataType mass_;

public:
    // Default constructor: Needed by Kokkos < 4.0
    PORTABLE_FUNCTION constexpr Isotope()
        : sza_(0)
        , mass_(0)
    {}

    // Constructor with SZA, mass
    PORTABLE_FUNCTION constexpr Isotope(SZA const sza, DataType const m)
        : sza_(sza)
        , mass_(m)
    {}

    // Constructor with int(SZA), mass
    PORTABLE_FUNCTION constexpr Isotope(int const sza, DataType const m)
        : sza_(sza)
        , mass_(m)
    {}

    // Constructor with Z, A, mass
    PORTABLE_FUNCTION constexpr Isotope(int const Z, int const A, DataType const m)
        : sza_(Z, A)
        , mass_(m)
    {}

    // Constructor with S, Z, A, mass
    PORTABLE_FUNCTION constexpr Isotope(int const S, int const Z, int const A, DataType const m)
        : sza_(Z, A, S)
        , mass_(m)
    {}

    // Metastable state
    PORTABLE_FUNCTION constexpr auto S() const { return sza_.S(); }

    // Number of protons
    PORTABLE_FUNCTION constexpr auto Z() const { return sza_.Z(); }

    // Number of neutrons
    PORTABLE_FUNCTION constexpr auto N() const { return A() - Z(); }

    // Number of nucleons (protons + neutrons)
    PORTABLE_FUNCTION constexpr auto A() const { return sza_.A(); }

    // SZA
    PORTABLE_FUNCTION constexpr auto sza() const { return sza_; }

    // Mass
    PORTABLE_FUNCTION constexpr auto mass() const { return mass_; }

    // Equality
    PORTABLE_FUNCTION constexpr bool operator==(Isotope<DataType> const & other) const
    {
        bool equal = true;
        equal = equal && (this->sza_ == other.sza_);
        equal = equal && (this->mass_ == other.mass_);
        return equal;
    }
    PORTABLE_FUNCTION constexpr bool operator!=(Isotope<DataType> const & other) const
    {
        return !(*this == other);
    }

    // Inequality
    PORTABLE_FUNCTION constexpr bool operator<(Isotope<DataType> const & other) const
    {
        if (this->sza() == other.sza()) {
            return this->mass() < other.mass();
        } else {
            return this->sza() < other.sza();
        }
    }
    PORTABLE_FUNCTION constexpr bool operator>=(Isotope<DataType> const & other) const
    {
        if (this->sza() == other.sza()) {
            return this->mass() >= other.mass();
        } else {
            return this->sza() >= other.sza();
        }
    }
    PORTABLE_FUNCTION constexpr bool operator<=(Isotope<DataType> const & other) const
    {
        if (this->sza() == other.sza()) {
            return this->mass() <= other.mass();
        } else {
            return this->sza() <= other.sza();
        }
    }
    PORTABLE_FUNCTION constexpr bool operator>(Isotope<DataType> const & other) const
    {
        if (this->sza() == other.sza()) {
            return this->mass() > other.mass();
        } else {
            return this->sza() > other.sza();
        }
    }
};

// Output streaming
template <typename DataType>
std::ostream & operator<<(std::ostream & out, Isotope<DataType> const & isotope)
{
    out << "Isotope(Z=" << isotope.Z() << ",A=" << isotope.A() << ")";
    return out;
}

// ================================================================================================

// The ReactionIsotope class is the base upon which Reactant and Product are built
template <typename DataType>
class ReactionIsotope : public Isotope<DataType>
{
private:
    // What is the index for this isotope in the isotope list?
    int index_;

public:
    // Default constructor: Needed by Kokkos < 4.0
    PORTABLE_FUNCTION constexpr ReactionIsotope()
        : Isotope<DataType>()
        , index_{0}
    {}

    // Constructor
    PORTABLE_FUNCTION constexpr ReactionIsotope(Isotope<DataType> const & iso, int const & index)
        : Isotope<DataType>(iso)
        , index_(index)
    {
        // What would a negative index even mean?
        assert(PortsOfCall::Robust::check_nonnegative(index));
    }

    // Accessor to get the index
    PORTABLE_FUNCTION constexpr int index() const { return index_; }

    // Comparison for the base class, to make comparisons easier for the derived classes
    PORTABLE_FUNCTION constexpr bool operator==(ReactionIsotope<DataType> const & other) const
    {
        bool equal = Isotope<DataType>::operator==(other);
        equal = equal && (this->index_ == other.index());
        return equal;
    }
};

// Output streaming
template <typename DataType>
std::ostream & operator<<(std::ostream & out, ReactionIsotope<DataType> const & isotope)
{
    out << "ReactionIsotope(Z=" << isotope.Z() << ",A=" << isotope.A() << ")";
    return out;
}

// ================================================================================================

} // end namespace nautilus

#endif // #ifndef NAUTILUS_ISOTOPE_HPP
