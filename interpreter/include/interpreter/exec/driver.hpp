#pragma once
#include <stack>
#include <string_view>

#include "interpreter/ast.hpp"
#include "interpreter/exec/global_ctx.hpp"
#include "interpreter/expr/expr.hpp"
#include "robot/robot.hpp"
#include "var/var.hpp"

namespace exec {
class Driver {
   public:
    Driver() = default;
    unsigned initialize(std::string_view program_filename,
                        std::unique_ptr<robot::Robot> robot,
                        bool trace_parsing = false,
                        bool trace_scanning = false);
    unsigned exec_next();
    unsigned get_next_lineno() const noexcept;
    std::optional<var::var_type> get_var(std::string_view var_name);
    std::optional<var::var_type> get_env();
    std::string_view get_curr_task_name();

   private:
    ast::Ast parse(bool trace_parsing, bool trace_scanning);
    void scan_begin(bool trace_scanning);
    void scan_end();
    ast::Expr *stack_top_exe();
    unsigned stack_top_line() const noexcept;

    std::stack<ast::Expr *> _exec_stack;
    GlobalCtx _ctx;
    std::string _pg;
};
}  // namespace exec
