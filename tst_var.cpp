#include <iostream>

#include "var/binary_op.hpp"
#include "var/bool.hpp"
#include "var/unary_op.hpp"
#include "var/var.hpp"
#include "var/var_ops.hpp"

int main() {
    var::var_type a = var::Var<int>(5, {4, 2, 3});
    var::var_type d = var::Var<int>(3, {4, 2, 3});
    var::var_type c = var::Var<int>(2, std::vector<unsigned>());
    std::vector<var::var_type> vec;
    vec.emplace_back(a);
    vec.emplace_back(d);
    vec.emplace_back(c);
    vec.emplace_back(var::idx(a, {1, 2, 3}));

    std::cout << std::endl;
    var::print(var::operation<var::IntegerMulOp>(vec[0], vec[1]));
    var::print(var::assign(vec[3], vec[2]));
    var::print(a);
    var::print(d);
    std::cout << std::endl;
    var::print(var::mx_operation<var::IntegerEq>(a));
    var::print(var::operation<var::IntegerMulOp>(a, d));
    std::cout << std::endl;

    //!!! idx should return ref
    //   var::assign(var::idx(var::execute(a), {1, 2, 2}),  c);
    //    var::assign(var::idx(a, {1, 2, 2}),  c);
    std::cout << "a[1, 2, 2] = c" << std::endl;
    var::print(a);

    std::cout << "c: ";
    var::print(c);
    var::assign(c, var::idx(a, {1, 2, 3}));
    std::cout << "c = a[1]" << std::endl;
    std::cout << "c: ";
    var::print(c);

    // var::var_type e = var::Var<int>(0, {2,3});
    //     var::print(e);
    // e = reduce(a, 1, 4);
    //     var::print(e);

    var::var_type e = var::Var<int>(3, {2, 3, 2});
    var::var_type f = var::Var<int>(0, {2, 5, 2});
    var::print(e);
    var::assign(f, extend(e, 2, 2));
    var::print(f);
    var::print(var::size(f));

    std::cout << std::endl;

    var::var_type b = var::Var<bool_t>(false, {2, 3, 2});
    var::var_type h = var::Var<bool_t>(true, {2, 3, 2});
    var::print(b);
    var::print(h);
    var::print(var::not_op(b));
    var::print(var::operation<var::LogicalAnd>(b, h));
    // var::print(var::operation<var::IntegerDivOp>(e, f));

    std::cout << std::endl;
    var::print(var::mx_operation<var::LogicalTrue>(b));
    var::print(var::mx_operation<var::LogicalTrue>(h));

    //    var::print(b);
}
