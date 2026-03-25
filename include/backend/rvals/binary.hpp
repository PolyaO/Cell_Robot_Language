#pragma once

#include <functional>

#include "backend/rvals/rval.hpp"

namespace ast {
class Binary {
   public:
    using Op = std::function<Var(const Var &, const Var &)>;

    Binary(unsigned idx1, unsigned idx2, Op op)
        : _idxs{idx1, idx2}, _op(std::move(op)) {}

    Var execute(ExecCtx &ctx) const;

   private:
    unsigned _idxs[2];
    Op _op;

};

}  // namespace ast
