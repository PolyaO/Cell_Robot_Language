#pragma once
#include <stack>
#include <string_view>

#include "interpreter/rvals/rval.hpp"
#include "interpreter/ast.hpp"
#include "interpreter/exec/global_ctx.hpp"

namespace exec {
class Driver {
   public:
    Driver() = default;
    unsigned initialize(std::string_view robot_filename,
                    std::string_view program_filename,
                    bool trace_parsing = false, bool trace_scanning = false);
    unsigned exec_next();
    std::optional<var::var_type> get_var(std::string_view var_name);
    std::string_view get_curr_task_name();

   private:
    ast::Ast parse(bool trace_parsing, bool trace_scanning);
    void scan_begin(bool trace_scanning);
    void scan_end();
    ast::expr *stack_top_exe();
    unsigned stack_top_line();

    std::stack<ast::expr *> _exec_stack;
    GlobalCtx _ctx;
    std::string _pg;
};
}  // namespace driver
