/* the Nuclide and IndexedNuclide classes

Classes used in multiple XCAP libraries to represent a nuclide.  The Nuclide class is the identity
(SZA) and mass of the nuclide.  IndexedNuclide is a superset that adds an index for tracking
nuclides within ordered containers.
*/

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
    [[nodiscard]] PORTABLE_FUNCTION constexpr auto S() const { return sza_.S(); }

    // Number of protons
    [[nodiscard]] PORTABLE_FUNCTION constexpr auto Z() const { return sza_.Z(); }

    // Number of neutrons
    [[nodiscard]] PORTABLE_FUNCTION constexpr auto N() const { return A() - Z(); }

    // Number of nucleons (protons + neutrons)
    [[nodiscard]] PORTABLE_FUNCTION constexpr auto A() const { return sza_.A(); }

    // SZA
    [[nodiscard]] PORTABLE_FUNCTION constexpr auto sza() const { return sza_; }

    // Mass
    [[nodiscard]] PORTABLE_FUNCTION constexpr auto mass() const { return mass_; }

    // Comparison
    [[nodiscard]] PORTABLE_FUNCTION constexpr bool operator==(
        Nuclide<DataType> const & other) const = default;
    [[nodiscard]] PORTABLE_FUNCTION constexpr std::partial_ordering operator<=>(
        Nuclide<DataType> const & other) const
    {
        if (this->sza() == other.sza()) {
            return this->mass() <=> other.mass();
        } else {
            return this->sza() <=> other.sza();
        }
    }
};

// Alias for compatibility with original Singe names
template <typename DataType>
using Isotope [[deprecated]] = Nuclide<DataType>;

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
    [[nodiscard]] PORTABLE_FUNCTION constexpr int index() const { return index_; }

    // Comparison for the base class, to make comparisons easier for the derived classes
    [[nodiscard]] PORTABLE_FUNCTION constexpr bool operator==(
        IndexedNuclide<DataType> const & other) const = default;
};

// Alias for compatibility with original Singe names
template <typename DataType>
using ReactionIsotope [[deprecated]] = Nuclide<DataType>;

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
