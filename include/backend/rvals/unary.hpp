#pragma once

#include <functional>

#include "backend/rvals/rval.hpp"

namespace ast {
class Unary {
   public:
    using Op = std::function<Var(const Var &)>;

    Unary(unsigned idx, Op op) : _idx(idx), _op(std::move(op)) {}

    Var execute(ExecCtx &ctx) const;
   private:
    unsigned _idx;
    Op _op;

};
}  // namespace ast
