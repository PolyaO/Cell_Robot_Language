#pragma once
#include <string>

namespace var {
struct IntegerEq {
    constexpr static std::string op_name = "eq";
    constexpr static int fn(int a) { return a == 0; };
};
struct IntegerNeq {
    constexpr static std::string op_name = "eq";
    constexpr static int fn(int a) { return a != 0; };
};
struct IntegerLt {
    constexpr static std::string op_name = "lt";
    constexpr static int fn(int a) { return a < 0; };
};
struct IntegerGt {
    constexpr static std::string op_name = "gt";
    constexpr static int fn(int a) { return a > 0; };
};
struct IntegerLte {
    constexpr static std::string op_name = "lte";
    constexpr static int fn(int a) { return a <= 0; };
};
struct IntegerGte {
    constexpr static std::string op_name = "gte";
    constexpr static int fn(int a) { return a >= 0; };
};

struct LogicalTrue {
    constexpr static std::string op_name = "true";
    constexpr static bool fn(bool a) { return a; };
};

struct LogicalFalse {
    constexpr static std::string op_name = "false";
    constexpr static bool fn(bool a) { return !a; };
};

template <class T>
concept IntegerUnaryOp = requires {
    { T::op_name } -> std::convertible_to<std::string>;
    { T::fn(int{}) } -> std::same_as<int>;
};

template <class T>
concept LogicalUnaryOp = requires {
    { T::op_name } -> std::convertible_to<std::string>;
    { T::fn(bool{}) } -> std::same_as<bool>;
};
static_assert(LogicalUnaryOp<LogicalFalse>);
static_assert(!IntegerUnaryOp<LogicalFalse>);

}  // namespace var
