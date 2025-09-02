#ifndef NAUTILUS_TOKENIZE_HPP
#define NAUTILUS_TOKENIZE_HPP

#include "ports-of-call/portability.hpp"

#include <array>
#include <string>
#include <string_view>

namespace nautilus::tag {

// ================================================================================================

inline std::array<std::string, 3> tokenize_nuclide(const std::string_view sv)
{
    // A nuclide consists of three tokens:
    // -- an atomic name or symbol: all letters, optionally a trailing hyphen
    // -- an atomic mass number: all digits
    // -- an excited state annotation:
    //    -- may be empty
    //    -- if non-empty, it starts with a letter and runs to the end of the string
    // Here tokenization is based on:
    // -- everything before the first number
    // -- the complete first number
    // -- everything after the first number
    // Further processing is format-dependent, and therefore deferred to the specific format
    std::string token0 = "";
    std::string token1 = "";
    std::string token2 = "";
    const auto pos1 = sv.find_first_of("0123456789");
    token0 = sv.substr(0, pos1);
    if (pos1 != std::string_view::npos) {
        const auto pos2 = sv.find_first_not_of("0123456789", pos1);
        if (pos2 != std::string_view::npos) {
            token1 = sv.substr(pos1, pos2 - pos1);
            token2 = sv.substr(pos2);
        } else {
            token1 = sv.substr(pos1);
        }
    }
    return {token0, token1, token2};
}

// ================================================================================================

} // end namespace nautilus::tag

#endif // #ifndef NAUTILUS_TOKENIZE_HPP
