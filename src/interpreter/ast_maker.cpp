#include "interpreter/ast_maker.hpp"

#include <algorithm>
#include <backend/exprs.hpp>
#include <string_view>
#include <variant>

#include "backend/rvals/rval.hpp"
#include "backend/rvals/unary.hpp"
#include "backend/rvals/var/binary_op.hpp"
#include "backend/rvals/var/unary_op.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include "interpreter/defs.hpp"
#include "interpreter/exceptions/build_exceptions.hpp"

ast::AstMaker::var_dict_t::const_iterator ast::AstMaker::find_var(
    std::string_view var) {
    return std::find_if(_variables_avaliable.begin(),
                        _variables_avaliable.end(),
                        [var](auto &v) { return std::get<0>(v) == var; });
}

ast::AstMaker::var_dict_t::const_iterator ast::AstMaker::find_var(
    unsigned assign_idx) {
    return std::find_if(
        _variables_avaliable.begin(), _variables_avaliable.end(),
        [assign_idx](auto &v) { return std::get<2>(v) == assign_idx; });
}

void ast::AstMaker::add_var(std::string_view str, unsigned var_idx,
                            unsigned assign_idx) {
    _variables_avaliable.emplace_back(str, var_idx, assign_idx);
}

void ast::AstMaker::check_for_varname_redeclaration(std::string_view var_name,
                                                    unsigned line) {
    auto it = find_var(var_name);
    if (it != _variables_avaliable.end()) {
        auto &var = _ast.get_rval(std::get<1>(*it));
        unsigned redecl_line;
        if (std::holds_alternative<var::var_type>(var)) {
            redecl_line =
                std::get<Assign>(*_ast.get_expr(std::get<2>(*it))).get_line();
        } else {
            redecl_line =
                std::get<Ref>(_ast.get_rval(std::get<1>(*it))).get_line();
        }
        throw VarRedeclare(var_name, line, redecl_line);
    }
}

void ast::AstMaker::check_for_taskname_redeclaration(
    std ::string_view task_name, unsigned line) {
    auto task_idx = _ast.get_task_idx(task_name);
    if (task_idx) {
        unsigned line = std::visit([](auto &task) { return task.get_line(); },
                                   *_ast.get_expr(task_idx.value()));
        throw TaskRedeclare(task_name, line, line);
    }
}

void ast::AstMaker::remove_scope_variables(const std::vector<unsigned> &vec) {
    var_dict_t::const_iterator it;
    for (auto idx : vec) {
        it = find_var(idx);
        if (it != _variables_avaliable.end()) _variables_avaliable.erase(it);
    }
}

ast::AstMaker::var_dict_t::const_iterator ast::AstMaker::check_for_var_unknown(
    std::string_view var_name, unsigned line) {
    auto it = find_var(var_name);
    if (it == _variables_avaliable.end()) throw VarUnknown(var_name, line);
    return it;
}

ast::variables_t ast::AstMaker::make_task_variables(
    ast::AstMaker::var_dict_t &&dict) {
    ast::variables_t res;
    for (auto &tup : dict) {
        res.emplace_back(std::move(std::get<0>(tup)), std::get<1>(tup));
    }
    dict.clear();
    return res;
}

ast::Ast ast::AstMaker::get_ast() { return std::move(_ast); }

std::variant<unsigned, std::vector<unsigned>> ast::AstMaker::make_arg_list() {
    if (_variables_avaliable.empty()) return 0u;
    return std::vector<unsigned>{};
}

void ast::AstMaker::add_to_arg_list(
    std::variant<unsigned, std::vector<unsigned>> &list, std::string_view arg,
    unsigned line) {
    auto it = find_var(arg);
    if (std::holds_alternative<unsigned>(list)) {
        if (it != _variables_avaliable.end()) throw ArgNameRepeat(arg, line);
        add_var(arg, _ast.make_rval<Ref>(0, line), -1);
        std::get<0>(list)++;
    } else {
        if (it == _variables_avaliable.end()) throw ArgNameUnknown(arg, line);
        std::get<1>(list).emplace_back(std::get<1>(*it));
    }
}

std::vector<unsigned> ast::AstMaker::make_dim_list() {
    return std::vector<unsigned>{};
}

void ast::AstMaker::add_to_dim_list(std::vector<unsigned> &list, int dim,
                                    unsigned line) {
    if (dim <= 0) throw WrongDim(dim, line);
    list.emplace_back(dim);
}

std::vector<unsigned> ast::AstMaker::make_stmts() {
    return std::vector<unsigned>{};
}

void ast::AstMaker::add_to_stmts(std::vector<unsigned> &stmts, unsigned expr_) {
    stmts.emplace_back(expr_);
}

void ast::AstMaker::set_politely_asked(unsigned expr_) {
    ast::expr *exp = _ast.get_expr(expr_);
    std::visit(
        [](auto &obj) {
            if constexpr (requires { obj.set_politely_asked(); })
                obj.set_politely_asked();
        },
        *exp);
}

unsigned ast::AstMaker::make_task(
    std::string_view task_name,
    const std::variant<unsigned, std::vector<unsigned>> &arg_list,
    std::vector<unsigned> &&exprs, std::string_view res, unsigned line) {
    check_for_taskname_redeclaration(task_name, line);
    auto it = find_var(res);
    if (it == _variables_avaliable.end())
        throw ResNameUnknown(res, task_name, line);
    auto task_idx = _ast.make_expr<Task>(std::move(exprs), std::get<1>(*it),
                                         std::get<unsigned>(arg_list), line);
    _ast.add_task_metainf(task_name, task_idx,
                          make_task_variables(std::move(_variables_avaliable)));
    return task_idx;
}

unsigned ast::AstMaker::make_findexit(std::vector<unsigned> &&exprs,
                                      unsigned line) {
    check_for_taskname_redeclaration("FINDEXIT", line);
    auto task_idx = _ast.make_expr<Task>(std::move(exprs), 0, 0, line);
    _ast.add_task_metainf("FINDEXIT", task_idx,
                          make_task_variables(std::move(_variables_avaliable)));
    return task_idx;
}

unsigned ast::AstMaker::make_assignement(std::string_view var_name,
                                         std::vector<unsigned> &&dim_list,
                                         unsigned rval_idx, unsigned line) {
    auto it = check_for_var_unknown(var_name, line);
    unsigned idx = std::get<1>(*it);
    if (!dim_list.empty())
        idx = _ast.make_rval<Idx>(idx, std::move(dim_list), line);
    return _ast.make_expr<Assign>(idx, rval_idx, line);
}

unsigned ast::AstMaker::make_do(
    std::string_view task_name,
    std::variant<unsigned, std::vector<unsigned>> &&arg_list, unsigned line) {
    auto idx = _ast.get_task_idx(task_name);
    if (!idx) throw TaskUnknown(task_name, line);
    return _ast.make_expr<Do>(idx.value(), std::move(std::get<1>(arg_list)),
                              line);
}

unsigned ast::AstMaker::make_for(std::string_view counter,
                                 std::string_view boundary,
                                 std::string_view step, unsigned stmt,
                                 unsigned line) {
    auto counter_idx = std::get<1>(*check_for_var_unknown(counter, line));
    auto boundary_idx = std::get<1>(*check_for_var_unknown(boundary, line));
    auto step_idx = std::get<1>(*check_for_var_unknown(step, line));
    const auto &counter_dim =
        var::get_dim(std::get<var::var_type>(_ast.get_rval(counter_idx)));
    auto curr_counter_idx =
        _ast.make_rval<var::var_type>(var::Var<int>(0, counter_dim));
    return _ast.make_expr<For>(counter_idx, curr_counter_idx, boundary_idx,
                               step_idx, stmt, line);
}

unsigned ast::AstMaker::make_switch(unsigned rval_idx, bool condition1,
                                    unsigned stmt1, bool condition2,
                                    unsigned stmt2, unsigned line) {
    if (condition1) {
        if (condition2) throw DoubleLogicLiteral("TRUE", line);
        return _ast.make_expr<Switch>(rval_idx, stmt1, stmt2, line);
    }
    if (!condition2 && stmt2) throw DoubleLogicLiteral("FALSE", line);
    return _ast.make_expr<Switch>(rval_idx, stmt2, stmt1, line);
}

unsigned ast::AstMaker::make_scope(std::vector<unsigned> &&exprs,
                                   unsigned line) {
    remove_scope_variables(exprs);
    return _ast.make_expr<Scope>(std::move(exprs), line);
}

unsigned ast::AstMaker::make_and(unsigned rval1_idx, unsigned rval2_idx,
                                 unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::LogicalAnd>, line);
}

unsigned ast::AstMaker::make_or(unsigned rval1_idx, unsigned rval2_idx,
                                unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::LogicalOr>, line);
}

unsigned ast::AstMaker::make_sum(unsigned rval1_idx, unsigned rval2_idx,
                                 unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::IntegerSumOp>, line);
}

unsigned ast::AstMaker::make_sub(unsigned rval1_idx, unsigned rval2_idx,
                                 unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::IntegerSubOp>, line);
}

unsigned ast::AstMaker::make_div(unsigned rval1_idx, unsigned rval2_idx,
                                 unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::IntegerDivOp>, line);
}

unsigned ast::AstMaker::make_mul(unsigned rval1_idx, unsigned rval2_idx,
                                 unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::IntegerMulOp>, line);
}

unsigned ast::AstMaker::make_not(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::not_op, line);
}

unsigned ast::AstMaker::make_mxtrue(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::LogicalTrue>,
                                 line);
}

unsigned ast::AstMaker::make_mxfalse(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::LogicalFalse>,
                                 line);
}

unsigned ast::AstMaker::make_mxeq(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerEq>,
                                 line);
}

unsigned ast::AstMaker::make_mxlt(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerLt>,
                                 line);
}

unsigned ast::AstMaker::make_mxgt(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerGt>,
                                 line);
}

unsigned ast::AstMaker::make_mxlte(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerLte>,
                                 line);
}

unsigned ast::AstMaker::make_mxgte(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerGte>,
                                 line);
}

unsigned ast::AstMaker::make_eleq(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerEq>,
                                 line);
}

unsigned ast::AstMaker::make_ellt(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerLt>,
                                 line);
}

unsigned ast::AstMaker::make_elgt(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerGt>,
                                 line);
}

unsigned ast::AstMaker::make_ellte(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerLte>,
                                 line);
}

unsigned ast::AstMaker::make_elgte(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerGte>,
                                 line);
}

unsigned ast::AstMaker::make_size(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::size, line);
}

unsigned ast::AstMaker::make_ref(std::string_view var_name, unsigned line) {
    auto idx = std::get<1>(*check_for_var_unknown(var_name, line));
    return _ast.make_rval<Ref>(idx, line);
}

unsigned ast::AstMaker::make_res(std::string_view task_name, unsigned line) {
    auto idx = _ast.get_task_idx(task_name);
    if (!idx) throw TaskUnknown(task_name, line);
    return _ast.make_rval<Res>(idx.value(), line);
}

unsigned ast::AstMaker::make_env(unsigned line) {
    return _ast.make_rval<Env>(line);
}

unsigned ast::AstMaker::make_idx(unsigned rval_idx,
                                 std::vector<unsigned> &&dim_list,
                                 unsigned line) {
    return _ast.make_rval<Idx>(rval_idx, std::move(dim_list), line);
}

