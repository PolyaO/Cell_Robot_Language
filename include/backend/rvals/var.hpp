#pragma once

#include <cstdint>
#include <span>
#include <variant>
#include <vector>

#include "backend/rvals/rval.hpp"


namespace ast {

class Var {
   public:

    struct VecIntVar {
        std::vector<int> values;
        std::vector<unsigned> dimensions;
    };

    struct VecBoolVar {
        std::vector<uint8_t> values;
        std::vector<unsigned> dimensions;
    };

    struct SpanBoolVar {
        std::span<uint8_t> values;
        std::span<unsigned> dimensions;
    };

    struct SpanIntVar {
        std::span<int> values;
        std::span<unsigned> dimensions;
    };

    using matrix_t = std::variant<VecBoolVar, VecIntVar, SpanBoolVar, SpanIntVar>;

    Var(int def_val, std::span<const unsigned> dim_list);
    Var(bool def_val, std::span<const unsigned> dim_list);
    std::span<const unsigned> get_dimensions();

    Var execute(ExecCtx &ctx) const;

    template <class Visitor>
    void visit(Visitor &&visitor) const {
        std::visit(std::forward<Visitor>(visitor), _mx);
    }
    void operator=(const Var &other);

    Var operator+(const Var &other) const;
    Var operator-(const Var &other) const;
    Var operator*(const Var &other) const;
    Var operator/(const Var &other) const;

    Var _and(const Var &other) const;
    Var _or(const Var &other) const;

    Var _idx(std::span<const unsigned> dim_list);
    Var _reduce(unsigned dim) const;
    Var _extend(unsigned dim) const;

    Var _not() const;
    Var _mxtrue() const;
    Var _mxfalse() const;
    Var _mxeq() const;
    Var _mxlt() const;
    Var _mxgt() const;
    Var _mxlte() const;
    Var _mxgte() const;
    Var _eleq() const;
    Var _ellt() const;
    Var _elgt() const;
    Var _ellte() const;
    Var _elgte() const;
    Var _size() const;
   private:
    Var(Var &other, std::vector<unsigned> &idx);
    void check_for_zeroes(std::span<const unsigned> dim_list);
    matrix_t _mx;
};
}  // namespace ast
