#pragma once

#include <functional>

#include "backend/rvals/rval.hpp"

namespace ast {
class Binary {
   public:
    using Op = std::function<var::var_type(const var::var_type &,
                                           const var::var_type &)>;

    Binary(unsigned idx1, unsigned idx2, Op op, unsigned line)
        : _idx1(idx1), _idx2(idx2), _line(line), _op(std::move(op)) {}

    var::var_type execute(ExecCtx &ctx) const;
    unsigned get_line();

   private:
    unsigned _idx1;
    unsigned _idx2;
    unsigned _line;
    Op _op;
};
}  // namespace ast
