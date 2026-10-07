#pragma once
#ifndef WCI_SOURCE_UTIL_CACHE_HPP
#define WCI_SOURCE_UTIL_CACHE_HPP

namespace wci
{
    /*
     * Cache that tracks the dirtiness of a value.
     */
    template<typename T>
    class Cache
    {
    private:
        struct {
            T value;
            bool dirty;
        } inner;

    public:
        constexpr Cache(const T& value);

        /*
         * Get mutable access to the held value. Marks the object dirty.
         */
        constexpr T& value() noexcept;

        /*
         * Get const access to the held value. Does not mark the object dirty.
         */
        constexpr const T& read() const noexcept;

        /*
         * Check wether the cache is valid (clean, not dirty).
         */
        constexpr bool valid() const noexcept;

        /*
         * Mark the object as dirty.
         */
        constexpr void dirty() noexcept;

        /*
         * Mark the object as clean.
         */
        constexpr void clean() noexcept;
    };
}

template<typename T>
inline constexpr wci::Cache<T>::Cache(const T& value) : inner{ value, true }
{}

template<typename T>
inline constexpr T& wci::Cache<T>::value() noexcept
{
    dirty();
    return inner.value;
}

template<typename T>
inline constexpr const T& wci::Cache<T>::read() const noexcept
{
    return inner.value;
}

template<typename T>
inline constexpr bool wci::Cache<T>::valid() const noexcept
{
    return !inner.dirty;
}

template<typename T>
inline constexpr void wci::Cache<T>::dirty() noexcept
{
    inner.dirty = true;
}

template<typename T>
inline constexpr void wci::Cache<T>::clean() noexcept
{
    inner.dirty = false;
}

#endif  // WCI_SOURCE_UTIL_CACHE_HPP
