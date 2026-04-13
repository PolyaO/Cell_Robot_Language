#include "interpreter/driver.hpp"

#include <optional>
#include <string_view>
#include <variant>

#include "backend/rvals/rval.hpp"
#include "interpreter/ast.hpp"
#include "interpreter/ast_maker.hpp"
#include "gen/lexer.hpp"
#include "gen/parser.hpp"

void driver::Driver::initialize(std::string_view robot_filename,
                                std::string_view program_filename,
                                bool trace_parsing, bool trace_scanning) {
    //_ctx.robot.initialize(robot_filename);
    _pg = program_filename;
    _ctx.ast = std::move(parse(trace_parsing, trace_scanning));
    _exec_stack.push(_ctx.ast.get_find_exit());
    _ctx.curr_task_idx = _ctx.ast.get_expr_idx(_exec_stack.top());
}

ast::Ast &&driver::Driver::parse(bool trace_parsing, bool trace_scanning) {
    ast::AstMaker maker;
    maker.loc.initialize(&_pg);
    scan_begin(trace_scanning);
    yy::parser parse(maker);
    parse.set_debug_level(trace_parsing);
    parse();
    scan_end();
    return maker.get_ast();
}

std::optional<var::var_type> driver::Driver::get_var(std::string_view var_name) {
    return _ctx.ast.get_variable(var_name, _ctx.curr_task_idx);
}

std::string_view driver::Driver::get_curr_task_name() {
    return _ctx.ast.get_task_name(_ctx.curr_task_idx);
}

std::optional<unsigned> driver::Driver::exec_next() {
    ast::expr *next = nullptr;
    if (_exec_stack.empty()) return std::optional<unsigned>();
    if ((next = std::visit(
             [this](auto &_expr) { return _expr.execute(this->_ctx); },
             *_exec_stack.top())) != nullptr) {
        _exec_stack.push(next);
    }
    else _exec_stack.pop();
    return std::visit([](auto &_expr) { return _expr.get_line(); },
                      *_exec_stack.top());
}

void driver::Driver::scan_begin(bool trace_scanning) {
    yyset_debug(trace_scanning);
    if (_pg.empty() || _pg == "-")
        throw std::runtime_error("no file to parse");

    else if (!(yyin = fopen(_pg.c_str(), "r"))) {
        std::cerr << "cannot open " << _pg << ": " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
}

void driver::Driver::scan_end() { fclose(yyin); }
void yy::parser::error(const yy::location &loc, const std::string &msg) {
    std::cerr << loc << ": " << msg << std::endl;
}

