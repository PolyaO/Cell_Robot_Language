#pragma once
#include <variant>
    template <typename Variant, typename T, std::size_t I = 0>
    constexpr bool is_variant_alternative() {
        if constexpr (I >= std::variant_size_v<Variant>) {
            return false;
        } else {
            return std::is_same<std::variant_alternative_t<I, Variant>, T>::value
                || is_variant_alternative<Variant, T, I + 1>();
        }
    }

