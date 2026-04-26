#pragma once
#include "var/binary_op.hpp"
#include "var/unary_op.hpp"
#include "var/var.hpp"

namespace var {
template <IntegerBinaryOp Op>
var_type operation(const var_type &a, const var_type &b) {
    return RETDOUBLEVISIT(a, v1, b, v2,
                          { return v1.template operation<Op>(v2); });
}

template <LogicalBinaryOp Op>
var_type operation(const var_type &a, const var_type &b) {
    return RETDOUBLEVISIT(a, v1, b, v2,
                          { return v1.template operation<Op>(v2); });
}

template <LogicalUnaryOp Op>
var_type operation(const var_type &a) {
    return std::visit([](auto &v) { return v.template operation<Op>(); }, a);
}

template <IntegerUnaryOp Op>
var_type operation(const var_type &a) {
    return std::visit([](auto &v) { return v.template operation<Op>(); }, a);
}

template <LogicalUnaryOp Op>
var_type mx_operation(const var_type &a) {
    return std::visit([](auto &v) { return v.template mx_operation<Op>(); }, a);
}

template <IntegerUnaryOp Op>
var_type mx_operation(const var_type &a) {
    return std::visit([](auto &v) { return v.template mx_operation<Op>(); }, a);
}
template <class T>
var_type assign_val(var_type &a, T b) {
    return std::visit([b](auto &v) { return v.template assign_v<T>(b); }, a);
}
template <class T>
var_type assign_val(var_type &&a, T b) {
    return std::visit([b](auto &v) { return v.template assign_v<T>(b); }, a);
}

var_type assign(var_type &a, const var_type &b);
var_type assign(var_type &&a, const var_type &b);
var_type not_op(const var_type &a);
var_type idx(var_type &&a, const std::vector<unsigned> &idx);
var_type reduce(const var_type &a, unsigned dim_idx = 1, unsigned change = 1);
var_type extend(const var_type &a, unsigned dim_idx = 1, unsigned change = 1);
var_type size(const var_type &a);
var_type copy_ref(var_type &a);

var_type idx(var_type &a, const std::vector<unsigned> &idx);
const var_type idx(const var_type &a, const std::vector<unsigned> &idx);

extern const var_type FALSE;
extern const var_type TRUE;

const std::vector<unsigned> &get_dim(const var_type &a);
void print(const var_type &a);
void logitize(var_type &a);
void digitize(var_type &a);
void logitize(var_type &&a);
void digitize(var_type &&a);
bool equal(const var_type &a, const var_type &b);

}  // namespace var
