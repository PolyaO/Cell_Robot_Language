#pragma once

#include <vector>

#include "backend/rvals/rval.hpp"
namespace ast {
class Ref {
   public:
    Ref(unsigned idx, unsigned line);
    var::var_type execute(ExecCtx &ctx) const;
    var::var_type &get(ExecCtx &ctx) const;
    unsigned get_line() const noexcept;
    unsigned get_idx() const noexcept;
    void set_idx(unsigned idx) noexcept;

   private:
    unsigned _idx;
    unsigned _line;
};
}  // namespace ast
