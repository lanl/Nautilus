#ifndef NAUTILUS_IDENTIFIERS_HPP
#define NAUTILUS_IDENTIFIERS_HPP

#include <array>
#include <string_view>

#include "ports-of-call/portability.hpp"

// ================================================================================================

namespace nautilus::tag {

// ================================================================================================

// TODO: How to indicate this is not part of the user interface?

template <std::size_t N>
class Identifiers
{
private:
    std::string_view symbol_;
    std::array<std::string_view, N> names_;

#define ENUMERATION(E) typename E, std::enable_if_t<std::is_enum<E>::value> * = nullptr

    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr void set(
        const std::string_view alternate_name, const E alt_standard)
    {
        const std::size_t alt_index = static_cast<std::size_t>(alt_standard);
        assert(alt_index < names_.size());
        names_[alt_index] = alternate_name;
    }
    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr std::string_view get(const E alt_standard) const
    {
        const std::size_t alt_index = static_cast<std::size_t>(alt_standard);
        assert(alt_index < names_.size());
        return names_[alt_index];
    }

public:
    // Constructor
    PORTABLE_FUNCTION constexpr Identifiers(
        const std::string_view symbol, const std::string_view standard_name)
        : symbol_{symbol}
    {
        for (auto & name : names_) {
            name = standard_name;
        }
    }
    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr Identifiers(
        const std::string_view symbol,
        const std::string_view standard_name,
        const std::string_view alternate_name,
        const E alt_standard)
        : Identifiers(symbol, standard_name)
    {
        set(alternate_name, alt_standard);
    }
    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr Identifiers(
        const std::string_view symbol,
        const std::string_view standard_name,
        const std::string_view alternate_name,
        const E alt_standard1,
        const E alt_standard2)
        : Identifiers(symbol, standard_name)
    {
        set(alternate_name, alt_standard1);
        set(alternate_name, alt_standard2);
    }

    // Fetch
    PORTABLE_FUNCTION constexpr std::string_view get_symbol() const { return symbol_; }
    // It's assumed that E(0) will be the default value.
    template <ENUMERATION(E)>
    PORTABLE_FUNCTION constexpr std::string_view get_name(const E standard = E(0)) const
    {
        return get(standard);
    }

    // Match
    PORTABLE_FUNCTION constexpr bool match_symbol(const std::string_view symbol) const
    {
        return symbol == symbol_;
    }
    PORTABLE_FUNCTION constexpr bool match_name(const std::string_view name) const
    {
        for (const auto & name0 : names_) {
            if (name0 == name) {
                return true;
            }
        }
        return false;
    }
};

// ================================================================================================

} // namespace nautilus::tag

#endif // NAUTILUS_IDENTIFIERS_HPP
