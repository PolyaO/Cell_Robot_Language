#include "backend/rvals/var/var_ops.hpp"

#include <iostream>
#include <variant>

#include "backend/rvals/var/bool.hpp"
#include "backend/rvals/var/var.hpp"

const var::var_type var::FALSE = var::Var<bool_t>(false, {1});
const var::var_type var::TRUE = var::Var<bool_t>(true, {1});

var::var_type var::assign(var_type &&a, const var_type &b) {
    return RETDOUBLEVISIT(a, v1, b, v2, { return v1.assign(v2); });
}
var::var_type var::assign(var_type &a, const var_type &b) {
    return RETDOUBLEVISIT(a, v1, b, v2, { return v1.assign(v2); });
}
var::var_type var::execute(var_type &a) {
    return std::visit([](auto &v) { return v.execute(); }, a);
}
var::var_type var::not_op(const var_type &a) {
    return std::visit([](auto &v) { return v._not(); }, a);
}
var::var_type var::idx(var_type &a, const std::vector<unsigned> &idx) {
    return std::visit([&](auto &v) { return v._idx(idx); }, a);
}
var::var_type var::idx(var_type &&a, const std::vector<unsigned> &idx) {
    return std::visit([&](auto &v) { return v._idx(idx); }, a);
}
var::var_type var::reduce(const var_type &a, unsigned dim_idx,
                          unsigned change) {
    return std::visit([&](auto &v) { return v._reduce(dim_idx, change); }, a);
}
var::var_type var::extend(const var_type &a, unsigned dim_idx,
                          unsigned change) {
    return std::visit([&](auto &v) { return v._extend(dim_idx, change); }, a);
}
var::var_type var::size(const var_type &a) {
    return std::visit([](auto &v) { return v._size(); }, a);
}
const std::vector<unsigned> &var::get_dim(const var_type &a) {
    return std::visit(
        [](auto &v) -> const std::vector<unsigned> & { return v.get_dim(); },
        a);
}
void var::print(const var::var_type &a) {
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

void var::logitize(var_type &a) {
    std::visit([](auto &v) { return v._logitize(); }, a);
}

void var::digitize(var_type &a) {
    std::visit([](auto &v) { return v._digitize(); }, a);
}

bool equal(const var::var_type &a, const var::var_type &b) {
    return RETDOUBLEVISIT(a, v1, b, v2, { return v1 == v2; });
}
