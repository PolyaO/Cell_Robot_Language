#include "interpreter/exec/driver.hpp"

#include <optional>
#include <string_view>
#include <variant>

#include "gen/lexer.hpp"
#include "gen/parser.hpp"
#include "interpreter/ast.hpp"
#include "interpreter/ast_maker.hpp"
#include "interpreter/exceptions/build_exceptions.hpp"
#include "interpreter/exprs/expr.hpp"
#include "interpreter/exprs/for.hpp"
#include "interpreter/exprs/switch.hpp"
#include "interpreter/exprs/task.hpp"
#include "interpreter/rvals/rval.hpp"
#include "var/var.hpp"

namespace exec {

unsigned Driver::initialize(std::string_view robot_filename,
                            std::string_view program_filename,
                            bool trace_parsing, bool trace_scanning) {
    //_ctx.robot.initialize(robot_filename);
    _pg = program_filename;
    _ctx.ast = parse(trace_parsing, trace_scanning);
    auto find_exit_info = _ctx.ast.find_task_metainf("FINDEXIT");
    if (!find_exit_info)
        throw InterpreterBuildError("No task with name \"FINDEXIT\" found\n");

    exec::LocalCtx new_ctx(find_exit_info->ctx_vars_number,
                           find_exit_info->ctx_scope_counters_number);
    new_ctx.set_task_idx(find_exit_info->task_idx);
    _ctx.push_curr_ctx(std::move(new_ctx));
    _exec_stack.push(_ctx.ast.get_expr(find_exit_info->task_idx));
    return stack_top_line();
}

ast::Ast Driver::parse(bool trace_parsing, bool trace_scanning) {
    ast::AstMaker maker;
    maker.loc.initialize(&_pg);
    scan_begin(trace_scanning);
    yy::parser parse(maker);
    parse.set_debug_level(trace_parsing);
    parse();
    scan_end();
    return maker.get_ast();
}

std::optional<var::var_type> Driver::get_var(std::string_view var_name) {
    if (_exec_stack.empty()) return std::nullopt;
    auto curr_task_inf =
        *_ctx.ast.find_task_metainf(_ctx.get_curr_ctx().get_task_idx());
    auto var_inf = std::find_if(
        curr_task_inf.vars_metainf.begin(), curr_task_inf.vars_metainf.end(),
        [&var_name](auto &inf) { return inf.var_name == var_name; });
    if (var_inf == curr_task_inf.vars_metainf.end())
        return std::optional<var::var_type>();
    auto var_ptr = _ctx.get_curr_ctx().get_var(var_inf->real_idx);
    if (!var_ptr) return std::optional<var::var_type>();
    return *var_ptr;
}

std::string_view Driver::get_curr_task_name() {
    auto &curr_task_inf =
        *_ctx.ast.find_task_metainf(_ctx.get_curr_ctx().get_task_idx());
    return curr_task_inf.task_name;
}

ast::expr *Driver::stack_top_exe() {
    return std::visit([this](auto &_expr) { return _expr.execute(this->_ctx); },
                      *_exec_stack.top());
}

unsigned Driver::stack_top_line() const noexcept {
    return std::visit([](auto &_expr) { return _expr.get_line(); },
                      *_exec_stack.top());
}

unsigned Driver::get_next_lineno() const noexcept {
    if (_exec_stack.empty()) return 0;
    return stack_top_line();
}

unsigned Driver::exec_next() {
    if (_exec_stack.empty()) return 0;
    ast::expr *next = stack_top_exe();
    if (!next) {
        _exec_stack.pop();
        return exec_next();
    }
    if (std::holds_alternative<ast::Task>(*next) |
        std::holds_alternative<ast::Scope>(*next) |
        std::holds_alternative<ast::For>(*next)) {
        _exec_stack.push(next);
        return exec_next();
    }
    if (std::holds_alternative<ast::Do>(*next) |
        std::holds_alternative<ast::Switch>(*next)) {
        next = std::visit([this](auto &n) { return n.execute(this->_ctx); },
                          *next);
        if (next) _exec_stack.push(next);
        return exec_next();
    }
    _exec_stack.push(next);
    return stack_top_line();
}

void Driver::scan_begin(bool trace_scanning) {
    yyset_debug(trace_scanning);
    if (_pg.empty() || _pg == "-")
        throw std::runtime_error("no file to parse");

    else if (!(yyin = fopen(_pg.c_str(), "r"))) {
        std::cerr << "cannot open " << _pg << ": " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
}

void Driver::scan_end() { fclose(yyin); }
}  // namespace exec
void yy::parser::error(const yy::location &loc, const std::string &msg) {
    std::cerr << loc << ": " << msg << std::endl;
}
