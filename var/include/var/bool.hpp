#pragma once
struct bool_t {
    bool _val;
    constexpr bool_t() : _val(false) {};
    constexpr bool_t(bool v) : _val(v) {};
    constexpr operator bool() const { return _val; }
};
