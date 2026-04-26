#pragma once

#include <memory>
#include <vector>

#include "interpreter/exprs/expr.hpp"

namespace ast {

class Do {
   public:
    Do(unsigned task_idx, std::vector<unsigned> &&arg_list, unsigned line);
    void set_politely_asked() noexcept;
    void set_task_idx(unsigned idx) noexcept;
    bool is_politely_asked() const noexcept;
    unsigned get_line() const noexcept;
    expr *execute(exec::GlobalCtx &ctx) const;

   private:
    std::unique_ptr<unsigned[]> _arg_list;
    unsigned _args_num;
    unsigned _task_idx;
    unsigned _line;
    bool _is_politely_asked = false;
    void throw_if_wrong_args_num(unsigned expected) const;
};
}  // namespace ast
