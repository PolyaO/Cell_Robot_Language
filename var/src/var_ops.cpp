#include "var/var_ops.hpp"

#include <iostream>
#include <variant>

#include "var/bool.hpp"
#include "var/var.hpp"

namespace var {
const var_type FALSE = Var<bool_t>(false, {1});
const var_type TRUE = Var<bool_t>(true, {1});

var_type assign(var_type &&a, const var_type &b) {
    return RETDOUBLEVISIT(a, v1, b, v2, { return v1.assign(v2); });
}
var_type assign(var_type &a, const var_type &b) {
    return RETDOUBLEVISIT(a, v1, b, v2, { return v1.assign(v2); });
}
var_type not_op(const var_type &a) {
    return std::visit([](auto &v) { return v._not(); }, a);
}
var_type idx(var_type &a, const std::vector<unsigned> &idx) {
    return std::visit([&](auto &v) { return v._idx(idx); }, a);
}
const var_type idx(const var_type &a, const std::vector<unsigned> &idx) {
    return std::visit([&](const auto &v) { return v._idx(idx); }, a);
}
var_type idx(var_type &&a, const std::vector<unsigned> &idx) {
    return std::visit([&](auto &v) { return v._idx(idx); }, a);
}
var_type reduce(const var_type &a, unsigned dim_idx, unsigned change) {
    return std::visit([&](auto &v) { return v._reduce(dim_idx, change); }, a);
}
var_type extend(const var_type &a, unsigned dim_idx, unsigned change) {
    return std::visit([&](auto &v) { return v._extend(dim_idx, change); }, a);
}
var_type size(const var_type &a) {
    return std::visit([](auto &v) { return v._size(); }, a);
}
var_type copy_ref(var_type &a) {
    return std::visit([](auto &v) { return v.copy_ref(); }, a);
}
const std::vector<unsigned> &get_dim(const var_type &a) {
    return std::visit(
        [](auto &v) -> const std::vector<unsigned> & { return v.get_dim(); },
        a);
}
void print(const var_type &a) {
    std::visit(
        [](auto &v) {
            auto val = v.get_val();
            auto dim = v.get_dim();
            std::visit(
                [](auto &mx) {
                    std::cout << "val: ";
                    for (auto &e : mx) std::cout << e << ", ";
                    std::cout << std::endl;
                },
                val);
            std::cout << "dim: ";
            for (auto &e : dim) std::cout << e << ", ";
            std::cout << std::endl;
        },
        a);
}

void logitize(var_type &a) {
    std::visit([](auto &v) { return v._logitize(); }, a);
}

void digitize(var_type &a) {
    std::visit([](auto &v) { return v._digitize(); }, a);
}

void logitize(var_type &&a) {
    std::visit([](auto &v) { return v._logitize(); }, a);
}

void digitize(var_type &&a) {
    std::visit([](auto &v) { return v._digitize(); }, a);
}
bool equal(const var_type &a, const var_type &b) {
    return RETDOUBLEVISIT(a, v1, b, v2, { return v1 == v2; });
}
}  // namespace var
