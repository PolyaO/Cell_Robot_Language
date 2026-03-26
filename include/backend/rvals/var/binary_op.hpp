#pragma once
#include <concepts>
#include <string>

namespace var {
struct IntegerSumOp {
    constexpr static std::string op_name = "sum";
    constexpr static int fn(int a, int b) { return a + b; };
};
struct IntegerSubOp {
    constexpr static std::string op_name = "subtraction";
    constexpr static int fn(int a, int b) { return a - b; };
};
struct IntegerDivOp {
    constexpr static std::string op_name = "division";
    constexpr static int fn(int a, int b) { return a / b; };
};
struct IntegerMulOp {
    constexpr static std::string op_name = "multiplication";
    constexpr static int fn(int a, int b) { return a * b; };
};
struct LogicalAnd {
    constexpr static std::string op_name = "and";
    constexpr static bool fn(bool a, bool b) { return a && b; };
};
struct LogicalOr {
    constexpr static std::string op_name = "or";
    constexpr static bool fn(bool a, bool b) { return a || b; };
};

template <class T>
concept IntegerBinaryOp = requires {
    { T::op_name } -> std::convertible_to<std::string>;
    { T::fn(int{}, int{}) } -> std::same_as<int>;
};

template <class T>
concept LogicalBinaryOp = requires {
    { T::op_name } -> std::convertible_to<std::string>;
    { T::fn(bool{}, bool{}) } -> std::same_as<bool>;
};

}  // namespace var
