#ifndef NAUTILUS_NUCLIDE_HPP
#define NAUTILUS_NUCLIDE_HPP

#include "nautilus/SZA.hpp"

#include "ports-of-call/portability.hpp"
#include "ports-of-call/robust_utils.hpp"

#include <cassert>
#include <iostream>

namespace nautilus {

// ================================================================================================

template <typename DataType>
struct Nuclide {
private:
    // The SZA value
    /*
    Handles S, Z, and A collectively and communicates with things that prefer the unified SZA
    concept.
    */
    SZA sza_;

    // The mass of the nuclide
    DataType mass_;

public:
    // Default constructor: Needed by Kokkos < 4.0
    PORTABLE_FUNCTION constexpr Nuclide()
        : sza_(0)
        , mass_(0)
    {}

    // Constructor with SZA, mass
    PORTABLE_FUNCTION constexpr Nuclide(SZA const sza, DataType const m)
        : sza_(sza)
        , mass_(m)
    {}

    // Constructor with int(SZA), mass
    PORTABLE_FUNCTION constexpr Nuclide(int const sza, DataType const m)
        : sza_(sza)
        , mass_(m)
    {}

    // Constructor with Z, A, mass
    PORTABLE_FUNCTION constexpr Nuclide(int const Z, int const A, DataType const m)
        : sza_(Z, A)
        , mass_(m)
    {}

    // Constructor with S, Z, A, mass
    PORTABLE_FUNCTION constexpr Nuclide(int const S, int const Z, int const A, DataType const m)
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
    PORTABLE_FUNCTION constexpr bool operator==(Nuclide<DataType> const & other) const
    {
        bool equal = true;
        equal = equal && (this->sza_ == other.sza_);
        equal = equal && (this->mass_ == other.mass_);
        return equal;
    }
    PORTABLE_FUNCTION constexpr bool operator!=(Nuclide<DataType> const & other) const
    {
        return !(*this == other);
    }

    // Inequality
    PORTABLE_FUNCTION constexpr bool operator<(Nuclide<DataType> const & other) const
    {
        if (this->sza() == other.sza()) {
            return this->mass() < other.mass();
        } else {
            return this->sza() < other.sza();
        }
    }
    PORTABLE_FUNCTION constexpr bool operator>=(Nuclide<DataType> const & other) const
    {
        return !(*this < other);
    }
    PORTABLE_FUNCTION constexpr bool operator<=(Nuclide<DataType> const & other) const
    {
        if (this->sza() == other.sza()) {
            return this->mass() <= other.mass();
        } else {
            return this->sza() <= other.sza();
        }
    }
    PORTABLE_FUNCTION constexpr bool operator>(Nuclide<DataType> const & other) const
    {
        return !(*this <= other);
    }
};

// **DEPRECATED**
// Alias for compatibility with original Singe names
template <typename DataType>
using Isotope = Nuclide<DataType>;

// Output streaming
template <typename DataType>
std::ostream & operator<<(std::ostream & out, Nuclide<DataType> const & nuclide)
{
    out << "Nuclide(Z=" << nuclide.Z() << ",A=" << nuclide.A() << ")";
    return out;
}

// ================================================================================================

// The IndexedNuclide class extends Nuclide by adding an index, for accessing data in
// consistently-ordered containers.
template <typename DataType>
class IndexedNuclide : public Nuclide<DataType>
{
private:
    // What is the index for this nuclide in the nuclide list?
    int index_;

public:
    // Default constructor: Needed by Kokkos < 4.0
    PORTABLE_FUNCTION constexpr IndexedNuclide()
        : Nuclide<DataType>()
        , index_{0}
    {}

    // Constructor
    PORTABLE_FUNCTION constexpr IndexedNuclide(Nuclide<DataType> const & iso, int const & index)
        : Nuclide<DataType>(iso)
        , index_(index)
    {
        assert(PortsOfCall::Robust::check_nonnegative(index));
    }

    // Accessor to get the index
    PORTABLE_FUNCTION constexpr int index() const { return index_; }

    // Comparison for the base class, to make comparisons easier for the derived classes
    PORTABLE_FUNCTION constexpr bool operator==(IndexedNuclide<DataType> const & other) const
    {
        bool equal = Nuclide<DataType>::operator==(other);
        equal = equal && (this->index_ == other.index());
        return equal;
    }
};

// **DEPRECATED**
// Alias for compatibility with original Singe names
template <typename DataType>
using ReactionIsotope = Nuclide<DataType>;

// Output streaming
template <typename DataType>
std::ostream & operator<<(std::ostream & out, IndexedNuclide<DataType> const & nuclide)
{
    out << "IndexedNuclide(Z=" << nuclide.Z() << ",A=" << nuclide.A() << ")";
    return out;
}

// ================================================================================================

} // end namespace nautilus

#endif // #ifndef NAUTILUS_NUCLIDE_HPP
