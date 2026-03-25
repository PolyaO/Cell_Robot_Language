#pragma once

#include <vector>
#include <memory>

#include "backend/exprs/expr.hpp"

namespace ast {

class Do {
   private:
    std::unique_ptr<unsigned[]> _arg_list;
    unsigned _args_num;
    unsigned _task_idx;
    unsigned _line;
    bool _is_politely_asked = false;

   public:
    Do(unsigned task_idx, std::vector<unsigned> &&arg_list, unsigned line);
    void set_politely_asked() noexcept;
    bool is_politely_asked() const noexcept;
    unsigned get_line() const noexcept;
    expr *execute(ExecCtx &ctx);
};
}  // namespace ast
