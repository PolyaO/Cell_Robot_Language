#pragma once

#include <functional>

#include "backend/rvals/rval.hpp"

namespace ast {
class Unary {
   public:
    using Op = std::function<var::var_type(const var::var_type &)>;

    Unary(unsigned idx, Op op, unsigned line);

    var::var_type execute(ExecCtx &ctx) const;
    unsigned get_line();

   private:
    unsigned _idx;
    unsigned _line;
    Op _op;
};
}  // namespace ast
