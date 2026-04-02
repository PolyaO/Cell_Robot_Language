#pragma once
#include <algorithm>
#include <format>
#include <numeric>
#include <ranges>
#include <span>
#include <string_view>
#include <variant>
#include <vector>

#include "backend/rvals/var/binary_op.hpp"
#include "backend/rvals/var/bool.hpp"
#include "backend/rvals/var/unary_op.hpp"

#define RETDOUBLEVISIT(_a, _name_a, _b, _name_b, _body)      \
    std::visit(                                              \
        [&](auto &_name_a) {                                 \
            return std::visit([&](auto &_name_b) _body, _b); \
        },                                                   \
        _a)

#define DOUBLEVISIT(_a, _name_a, _b, _name_b, _body) \
    std::visit(                                      \
        [&](auto &_name_a) { std::visit([&](auto &_name_b) _body, _b); }, _a)

#define THROW(_msg) throw std::runtime_error(_msg)

namespace var {

template <class T>
class Var;
using var_type = std::variant<Var<int>, Var<bool_t>>;
using dim_t = std::vector<unsigned>;

template <class T>
class Var {
   public:
    using dim_t = var::dim_t;
    using val_t = std::variant<std::span<T>, std::vector<T>>;

    Var(T def_val, const dim_t &dim);
    Var(dim_t &&dim, val_t &&val);
    Var(const Var<T> &other) noexcept = default;

    Var<T> &operator=(const Var<T> &other) = delete;
    Var<T> &operator=(Var<T> &&other) = default;

    template <class U>
    var_type assign(const Var<U> &other);

    template <IntegerBinaryOp Op, class U>
    var_type operation(const Var<U> &other) const;

    template <LogicalBinaryOp Op, class U>
    var_type operation(const Var<U> &other) const;

    template <IntegerUnaryOp Op>
    var_type operation() const;

    template <LogicalUnaryOp Op>
    var_type operation() const;

    template <LogicalUnaryOp Op>
    var_type mx_operation() const;
    template <IntegerUnaryOp Op>
    var_type mx_operation() const;
    var_type execute();

    var_type _not() const;
    var_type _idx(const dim_t &idx);
    var_type _reduce(unsigned dim_idx = 1, unsigned change = 1) const;
    var_type _extend(unsigned dim_idx = 1, unsigned change = 1) const;
    var_type _size() const;
    var_type idx(unsigned idx);

    const dim_t &get_dim() const;
    const val_t &get_val() const;

   private:
    template <class U>
    Var(Var<U>::dim_t &&dim, Var<U>::val_t &&val);
    void check_dims_zeroes(const dim_t &dim, std::string_view msg) const;
    void check_diff_dims(const dim_t &dim, std::string_view msg) const;
    void check_idx(const dim_t &idx) const;
    void check_dim_idx(unsigned idx, std::string_view msg) const;
    void delete_extra_ones(dim_t &dim) const;
    template <class InputIt>
    unsigned mul_dims(InputIt first, InputIt last) const;
    unsigned _val_size() const;
    dim_t make_dim_drop_1(const dim_t &dim) const;

    dim_t _dim;
    val_t _val;
};

// вспомогательные методы Var

template <class T>
void Var<T>::check_dims_zeroes(const dim_t &dim, std::string_view msg) const {
    if (std::ranges::find(dim, 0) != dim.end())
        THROW(std::format("Zero in dimensions list in {}", msg));
}

template <class T>
void Var<T>::check_diff_dims(const dim_t &dim, std::string_view msg) const {
    if (_dim != dim)
        THROW(
            std::format("Variables with different dimensions used in {}", msg));
}

template <class T>
void Var<T>::check_idx(const dim_t &idx) const {
    if (idx.size() > _dim.size())
        THROW("Idx sequence is longer than dimensions list");
    for (auto [real_idx, idx_seq] : std::views::enumerate(idx)) {
        if (idx_seq > _dim[real_idx])
            THROW(std::format("Idx is out of range {}", idx_seq));
        if (idx_seq == 0) THROW("Zero in idx list. Idxes start from 1");
    }
}

template <class T>
void Var<T>::check_dim_idx(unsigned dim_idx, std::string_view msg) const {
    if (dim_idx > _dim.size())
        THROW(std::format("Dimension idx is out of range in {}", msg));
    if (dim_idx == 0)
        THROW(std::format("Zero used as dimension in {}. Idx starts from 1",
                          msg));
}

template <class T>
void Var<T>::delete_extra_ones(dim_t &dim) const {
    std::erase(dim, 1);
    if (dim.empty()) dim.emplace_back(1);
}

template <class T>
Var<T>::dim_t Var<T>::make_dim_drop_1(const dim_t &dim) const {
    if (dim.size() == 1) {
        return {1};
    } else
        return dim_t(dim.begin() + 1, dim.end());
}

template <class T>
template <class InputIt>
unsigned Var<T>::mul_dims(InputIt first, InputIt last) const {
    return std::accumulate(first, last, 1,
                           [](unsigned a, unsigned b) { return a * b; });
}

template <class T>
unsigned Var<T>::_val_size() const {
    return std::visit([](auto &v) { return v.size(); }, _val);
}

// основные методы Var
template <class T>
Var<T>::Var(T def_val, const dim_t &dim) {
    check_dims_zeroes(dim, "var declaration");
    _dim = dim;
    delete_extra_ones(_dim);
    unsigned val_size = mul_dims(_dim.begin(), _dim.end());
    _val = std::vector<T>(val_size, def_val);
}

template <class T>
Var<T>::Var(dim_t &&dim, val_t &&val)
    : _dim(std::move(dim)) {
    _val = std::move(val);
    check_dims_zeroes(dim, "var declaration");
    delete_extra_ones(_dim);
    if (mul_dims(_dim.begin(), _dim.end()) != _val_size())
        THROW("in var declaration passed values don't match dimensions");
}

template <class T>
template <class U>
var_type Var<T>::assign(const Var<U> &other) {
    if constexpr (!std::is_same_v<T, U>)
        THROW("different types in assignement");
    else {
        check_diff_dims(other._dim, "assignement");
        DOUBLEVISIT(_val, v1, other._val, v2,
                    { std::ranges::copy(v2, v1.begin()); });
        _dim = other._dim;
        return (*this).execute();
    }
}

template <class T>
template <IntegerBinaryOp Op, class U>
var_type Var<T>::operation(const Var<U> &other) const {
    if constexpr (!std::is_same_v<T, int> || !std::is_same_v<U, int>)
        THROW(std::format("Boolean Matrix used in {}", Op::op_name));
    else {
        check_diff_dims(other._dim, Op::op_name);
        std::vector<int> res(_val_size(), 0);
        DOUBLEVISIT(_val, v1, other._val, v2, {
            for (auto &&[a, b, r] : std::ranges::zip_view(v1, v2, res)) {
                r = Op::fn(a, b);
            }
        });
        return Var<int>(dim_t(_dim), std::move(res));
    }
}

template <class T>
template <LogicalBinaryOp Op, class U>
var_type Var<T>::operation(const Var<U> &other) const {
    if constexpr (!std::is_same_v<T, bool_t> || !std::is_same_v<U, bool_t>)
        THROW(std::format("Integer Matrix used in {}", Op::op_name));
    else {
        check_diff_dims(other._dim, Op::op_name);
        std::vector<bool_t> res(_val_size(), false);
        DOUBLEVISIT(_val, v1, other._val, v2, {
            for (auto &&[a, b, r] : std::ranges::zip_view(v1, v2, res)) {
                r = Op::fn(a, b);
            }
        });
        return Var<bool_t>(dim_t(_dim), std::move(res));
    }
}

template <class T>
template <IntegerUnaryOp Op>
var_type Var<T>::operation() const {
    if constexpr (!std::is_same_v<T, int>)
        THROW(std::format("Boolean Matrix used in el{}", Op::op_name));
    else {
        std::vector<bool_t> res(_val_size(), false);
        std::visit(
            [&res](auto &v) {
                for (auto &&[e, r] : std::ranges::zip_view(v, res)) {
                    r = Op::fn(e);
                }
            },
            _val);
        return Var<bool_t>(dim_t(_dim), std::move(res));
    }
}

template <class T>
template <IntegerUnaryOp Op>
var_type Var<T>::mx_operation() const {
    if constexpr (!std::is_same_v<T, int>)
        THROW(std::format("Boolean Matrix used in mx{}", Op::op_name));
    else {
        unsigned count_1 = 0;
        unsigned count_2 = 0;
        std::visit(
            [&](auto &v) {
                for (auto &e : v) {
                    if (Op::fn(e))
                        count_1++;
                    else
                        count_2++;
                }
            },
            _val);
        bool res = count_1 > count_2;
        return Var<bool_t>({1}, std::vector<bool_t>(1, res));
    }
}

template <class T>
template <LogicalUnaryOp Op>
var_type Var<T>::mx_operation() const {
    if constexpr (!std::is_same_v<T, bool_t>)
        THROW(std::format("Integer Matrix used in mx{}", Op::op_name));
    else {
        unsigned count_1 = 0;
        unsigned count_2 = 0;
        std::visit(
            [&](auto &v) {
                for (auto &e : v) {
                    if (Op::fn(e))
                        count_1++;
                    else
                        count_2++;
                }
            },
            _val);
        bool res = count_1 > count_2;
        return Var<bool_t>({1}, std::vector<bool_t>(1, res));
    }
}

template <class T>
var_type Var<T>::_not() const {
    if constexpr (!std::is_same_v<T, bool_t>)
        THROW("Integer Matrix used in not");
    else {
        std::vector<bool_t> res(_val_size(), false);
        std::visit(
            [&res](auto &v) {
                for (auto &&[e, r] : std::ranges::zip_view(v, res)) {
                    r = !e;
                }
            },
            _val);
        return Var<bool_t>(dim_t(_dim), std::move(res));
    }
}

template <class T>
var_type Var<T>::execute() {
    dim_t dim = _dim;
    return std::visit(
        [&](auto &v) {
            return Var<T>(std::move(dim),
                          std::span<T>(v.begin(), v.end()));
        },
        _val);
}

template <class T>
var_type Var<T>::idx(unsigned idx) {
    unsigned cell_size = _val_size() / _dim[0];
    unsigned offset = cell_size * (idx - 1);
    dim_t dim = std::move(make_dim_drop_1(_dim));
    return std::visit(
        [&](auto &v) {
            return Var<T>(std::move(dim),
                          std::span<T>(v.begin() + offset, cell_size));
        },
        _val);
}

template <class T>
var_type Var<T>::_idx(const dim_t &idx) {
    check_idx(idx);
    auto res(std::move((*this).idx(idx[0])));
    for (auto i : idx | std::views::drop(1)) {
        res = std::move(std::visit([&](auto &r) { return r.idx(i); }, res));
    }
    return res;
}

template <class T>
var_type Var<T>::_reduce(unsigned dim_idx, unsigned change) const {
    check_dim_idx(dim_idx, "reduce");
    std::vector<T> val;
    dim_t dim = _dim;
    if (change == _dim[dim_idx - 1]) change--;
    if (change == 0) {
        std::visit([&](auto &v) { std::ranges::copy(v, val.begin()); }, _val);
        return Var<T>(std::move(dim), std::move(val));
    }

    unsigned del_cell_size = mul_dims(_dim.begin() + dim_idx, _dim.end());
    unsigned k = del_cell_size * change;
    unsigned n = del_cell_size * _dim[dim_idx - 1];
    unsigned m = _val_size() / n;

    if ((dim[dim_idx - 1] = dim[dim_idx - 1] - change) == 1 &&
        dim.size() != 1) {
        dim.erase(dim.begin() + dim_idx - 1);
    }
    val.reserve(m * (n - k));

    std::visit(
        [&](auto &v) {
            for (size_t i = 0; i < m; ++i) {
                auto row_start = v.begin() + i * n;
                auto row_end = row_start + (n - k);
                val.insert(val.end(), row_start, row_end);
            }
        },
        _val);

    return Var<T>(std::move(dim), std::move(val));
}

template <class T>
var_type Var<T>::_extend(unsigned dim_idx, unsigned change) const {
    check_dim_idx(dim_idx, "extend");
    std::vector<T> val;
    dim_t dim = _dim;
    if (change == 0) {
        std::visit([&](auto &v) { std::ranges::copy(v, val.begin()); }, _val);
        return Var<T>(std::move(dim), std::move(val));
    }

    unsigned ins_cell_size = mul_dims(_dim.begin() + dim_idx, _dim.end());
    unsigned k = ins_cell_size * change;
    unsigned n = ins_cell_size * _dim[dim_idx - 1];
    unsigned m = _val_size() / n;

    dim[dim_idx - 1] = dim[dim_idx - 1] + change;
    T def = T{};
    val.reserve(m * (n + k));

    std::visit(
        [&](auto &v) {
            for (size_t i = 0; i < m; ++i) {
                auto row_start = v.begin() + i * n;
                auto row_end = row_start + n;
                val.insert(val.end(), row_start, row_end);
                val.insert(val.end(), k, def);
            }
        },
        _val);

    return Var<T>(std::move(dim), std::move(val));
}

template <class T>
var_type Var<T>::_size() const {
    dim_t dim = {(unsigned)_dim.size()};
    std::vector<int> val(_dim.begin(), _dim.end());
    return Var<int>(std::move(dim), std::move(val));
}

template <class T>
const Var<T>::val_t &Var<T>::get_val() const {
    return _val;
}

template <class T>
const Var<T>::dim_t &Var<T>::get_dim() const {
    return _dim;
}
}  // namespace var
