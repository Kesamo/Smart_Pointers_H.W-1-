#pragma once

#include <type_traits>


template <class T>
class SharedPtr {
private:
    using type = std::remove_extent_t<T>;
};