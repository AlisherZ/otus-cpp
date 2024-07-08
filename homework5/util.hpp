#pragma once

#include <cstdint>
#include <tuple>
#include <string>

using GraphicalContext = std::string; // TODO GraphicalContext for form

template<std::size_t I = 0, typename... T>
std::uint32_t sum_elements(std::tuple<T...>& t) {
    if constexpr(I + 1 < sizeof...(T)) {
        return (sum_elements<I + 1>(t) << 8) + std::get<sizeof...(T) - I - 1>(t);
    }
    else {
        return static_cast<std::uint32_t>(std::get<0>(t));
    }
}