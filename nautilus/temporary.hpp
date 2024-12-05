/*****************************************************************************
 * This file is intended to be deleted when a new Ports-of-Call release is   *
 * available.  Do NOT rely on this unless you're okay with it disappearing   *
 * and having to switch to the new Ports-of-Call release yourself.           *
 *****************************************************************************/

#ifndef NAUTILUS_TEMPORARY_HPP
#define NAUTILUS_TEMPORARY_HPP

// type-safe check against 0 (prevents warnings about comparing an unsigned against 0)
template <typename T, std::enable_if_t<std::is_unsigned<T>::value, bool> = true>
PORTABLE_FUNCTION constexpr bool check_nonnegative(const T)
{
    return true;
}
template <typename T, std::enable_if_t<!std::is_unsigned<T>::value, bool> = true>
PORTABLE_FUNCTION constexpr bool check_nonnegative(const T t)
{
    return t >= 0;
}

#endif // #ifndef NAUTILUS_TEMPORARY_HPP
