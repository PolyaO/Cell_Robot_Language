#include "interpreter/ast_maker.hpp"

#include <algorithm>
#include <string_view>
#include <variant>

#include "interpreter/ast.hpp"
#include "interpreter/exceptions/build_exceptions.hpp"
#include "interpreter/expr/exprs.hpp"
#include "var/binary_op.hpp"
#include "var/unary_op.hpp"
#include "var/var_ops.hpp"
#define FAKE_DECLARE_IDX -1
namespace ast {

const AstMaker::VarInfo *AstMaker::VariablesDict::get_var_info(
    std::string_view var_name) const {
    auto it = std::find_if(
        _variables_avaliable.begin(), _variables_avaliable.end(),
        [var_name](auto &v) { return v.metainf.var_name == var_name; });
    if (it == _variables_avaliable.end())
        return nullptr;
    else
        return &*it;
}

void AstMaker::VariablesDict::add_var_info(VarInfo &&info) {
    _variables_avaliable.emplace_back(std::move(info));
}

void AstMaker::VariablesDict::remove_declared(
    const std::vector<unsigned> &exprs, Ast::vars_metainf_t &dst) {
    for (auto decl_idx : exprs) {
        auto it = std::find_if(
            _variables_avaliable.begin(), _variables_avaliable.end(),
            [decl_idx](auto &v) { return v.declare_idx == decl_idx; });
        if (it != _variables_avaliable.end()) {
            dst.emplace_back(std::move(it->metainf));
            _variables_avaliable.erase(it);
        }
    }
}

bool AstMaker::VariablesDict::empty() const {
    return _variables_avaliable.empty();
}

Ast::vars_metainf_t AstMaker::VariablesDict::clear() {
    Ast::vars_metainf_t res;
    for (auto &inf : _variables_avaliable) {
        res.emplace_back(std::move(inf.metainf));
    }
    _variables_avaliable.clear();
    return std::move(res);
}

Ast AstMaker::get_ast() { return std::move(_ast); }

void AstMaker::set_politely_asked(unsigned expr) {
    _ast.get_expr(expr)->set_politely_asked();
}

std::variant<unsigned, std::vector<unsigned>> AstMaker::make_arg_list() const {
    if (_variables_avaliable.empty()) return 0u;
    return std::vector<unsigned>{};
}

void AstMaker::add_to_arg_list(
    std::variant<unsigned, std::vector<unsigned>> &list, std::string_view arg,
    unsigned lineno) {
    if (std::holds_alternative<unsigned>(list)) {
        if (_variables_avaliable.get_var_info(arg))
            throw ArgNameRepeat(arg, lineno);
        unsigned ref_idx = _ast.make_rval<rvals::Ref>(lineno, _vars_number);
        Ast::VarMetainf m(std::string(arg), ref_idx, _vars_number);
        VarInfo inf(std::move(m), FAKE_DECLARE_IDX, lineno);
        _variables_avaliable.add_var_info(std::move(inf));
        std::get<unsigned>(list)++;
        _vars_number++;
    } else {
        auto arg_inf_ptr = _variables_avaliable.get_var_info(arg);
        if (!arg_inf_ptr) throw ArgNameUnknown(arg, lineno);
        std::get<1>(list).emplace_back(arg_inf_ptr->metainf.real_idx);
    }
}

std::vector<unsigned> AstMaker::make_dim_list() const {
    return std::vector<unsigned>{};
}

void AstMaker::add_to_dim_list(std::vector<unsigned> &list, int dim,
                               unsigned lineno) const {
    if (dim <= 0) throw WrongDim(dim, lineno);
    list.emplace_back(dim);
}

std::vector<unsigned> AstMaker::make_stmts() const {
    return std::vector<unsigned>{};
}

void AstMaker::add_to_stmts(std::vector<unsigned> &stmts,
                            unsigned expr_) const {
    stmts.emplace_back(expr_);
}

unsigned AstMaker::make_ret_res(std::string_view res_name, unsigned lineno) {
    auto var_info_ptr = _variables_avaliable.get_var_info(res_name);
    if (!var_info_ptr) throw VarUnknown(res_name, lineno);
    _met_res = true;
    return _ast.make_expr<exprs::RetRes>(lineno, false,
                                         var_info_ptr->metainf.real_idx);
}

unsigned AstMaker::make_task(
    std::string_view task_name,
    const std::variant<unsigned, std::vector<unsigned>> &arg_list,
    std::vector<unsigned> &&exprs, unsigned lineno) {
    auto task_inf_ptr = _ast.find_task_metainf(task_name);
    if (task_inf_ptr)
        throw TaskRedeclare(task_name, task_inf_ptr->decl_lineno, lineno);
    if (!_met_res) throw NoRetRes(task_name, lineno);
    unsigned res = _ast.make_expr<exprs::Scope>(lineno, true, std::move(exprs),
                                                _counters_number++);
    make_task_metainf(task_name, res, std::get<unsigned>(arg_list), lineno);
    return res;
}

void AstMaker::make_task_metainf(std::string_view task_name, unsigned task_idx,
                                 unsigned args_num, unsigned lineno) {
    _task_vars.append_range(_variables_avaliable.clear());
    Ast::TaskMetainf m;
    m.task_name = task_name;
    m.vars_metainf = std::move(_task_vars);
    m.decl_lineno = lineno;
    m.task_idx = task_idx;
    m.args_number = args_num;
    m.ctx_vars_number = _vars_number;
    m.ctx_counters_number = _counters_number;
    _vars_number = 0;
    _counters_number = 0;
    _met_res = false;
    _ast.add_task_metainf(std::move(m));
    process_task_calls(task_name);
    return;
}

unsigned AstMaker::make_findexit(std::vector<unsigned> &&exprs,
                                 unsigned lineno) {
    auto task_inf_ptr = _ast.find_task_metainf("FINDEXIT");
    if (task_inf_ptr)
        throw TaskRedeclare("FINDEXIT", task_inf_ptr->decl_lineno, lineno);
    unsigned res = _ast.make_expr<exprs::Scope>(lineno, true, std::move(exprs),
                                                _counters_number++);
    make_task_metainf("FINDEXIT", res, 0, lineno);
    return res;
}

void AstMaker::process_task_calls(std::string_view task_name) {
    for (auto &call : _unprocessed_task_calls) {
        if (call.task_name != task_name) {
            throw TaskUnknown(call.task_name, call.lineno);
        } else {
            _ast.get_expr(call.do_idx)
                ->set_task_idx(_ast.find_task_metainf(task_name)->task_idx);
        }
    }
    _unprocessed_task_calls.clear();
}

unsigned AstMaker::make_assignement(std::string_view var_name,
                                    std::vector<unsigned> &&dim_list,
                                    unsigned rval_idx, unsigned lineno) {
    auto var_info_ptr = _variables_avaliable.get_var_info(var_name);
    if (!var_info_ptr) throw VarUnknown(var_name, lineno);
    unsigned idx = var_info_ptr->metainf.ref_idx;
    if (!dim_list.empty()) {
        idx = _ast.make_rval<rvals::Idx>(lineno, idx, std::move(dim_list));
    }
    return _ast.make_expr<exprs::Assign>(lineno, false, idx, rval_idx);
}

unsigned AstMaker::make_do(
    std::string_view task_name,
    std::variant<unsigned, std::vector<unsigned>> &&arg_list, unsigned lineno) {
    unsigned idx = 0;
    auto m_ptr = _ast.find_task_metainf(task_name);
    if (m_ptr) idx = m_ptr->task_idx;
    auto do_idx =
        _ast.make_expr<exprs::Do>(lineno, false, idx, _counters_number++,
                                  std::move(std::get<1>(arg_list)));
    if (!idx) {
        _unprocessed_task_calls.emplace_back(
            TaskCallInfo(do_idx, lineno, std::string(task_name)));
    }
    return do_idx;
}

unsigned AstMaker::make_for(std::string_view counter, std::string_view boundary,
                            std::string_view step, unsigned stmt,
                            unsigned lineno) {
    auto counter_inf_ptr = _variables_avaliable.get_var_info(counter);
    if (!counter_inf_ptr) throw VarUnknown(counter, lineno);
    auto boundary_inf_ptr = _variables_avaliable.get_var_info(boundary);
    if (!boundary_inf_ptr) throw VarUnknown(boundary, lineno);
    auto step_inf_ptr = _variables_avaliable.get_var_info(step);
    if (!step_inf_ptr) throw VarUnknown(step, lineno);

    auto res = _ast.make_expr<exprs::For>(
        lineno, true, counter_inf_ptr->metainf.real_idx,
        boundary_inf_ptr->metainf.real_idx, step_inf_ptr->metainf.real_idx,
        stmt, _vars_number++, _counters_number, _counters_number + 1);
    _counters_number += 2;
    return res;
}

unsigned AstMaker::make_switch(unsigned rval_idx, bool condition1,
                               unsigned stmt1, bool condition2, unsigned stmt2,
                               unsigned lineno) {
    if (condition1) {
        if (condition2) throw DoubleLogicLiteral("TRUE", lineno);
        return _ast.make_expr<exprs::Switch>(lineno, false, rval_idx,
                                             _counters_number++, stmt1, stmt2);
    }
    if (!condition2) throw DoubleLogicLiteral("FALSE", lineno);
    return _ast.make_expr<exprs::Switch>(lineno, false, rval_idx,
                                         _counters_number++, stmt2, stmt1);
}

unsigned AstMaker::make_switch_no_tail(unsigned rval_idx, bool condition,
                               unsigned stmt,
                               unsigned lineno) {
    if (condition) {
        return _ast.make_expr<exprs::Switch>(lineno, false, rval_idx,
                                             _counters_number++, stmt, -1);
    }
        return _ast.make_expr<exprs::Switch>(lineno, false, rval_idx,
                                             _counters_number++,  -1, stmt);

}

unsigned AstMaker::make_scope(std::vector<unsigned> &&exprs, unsigned lineno) {
    _variables_avaliable.remove_declared(exprs, _task_vars);
    return _ast.make_expr<exprs::Scope>(lineno, true, std::move(exprs),
                                        _counters_number++);
}

unsigned AstMaker::make_and(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned lineno) {
    return _ast.make_rval<rvals::Binary>(lineno, rval1_idx, rval2_idx,
                                         var::operation<var::LogicalAnd>);
}

unsigned AstMaker::make_or(unsigned rval1_idx, unsigned rval2_idx,
                           unsigned lineno) {
    return _ast.make_rval<rvals::Binary>(lineno, rval1_idx, rval2_idx,
                                         var::operation<var::LogicalOr>);
}

unsigned AstMaker::make_sum(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned lineno) {
    return _ast.make_rval<rvals::Binary>(lineno, rval1_idx, rval2_idx,
                                         var::operation<var::IntegerSumOp>);
}

unsigned AstMaker::make_sub(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned lineno) {
    return _ast.make_rval<rvals::Binary>(lineno, rval1_idx, rval2_idx,
                                         var::operation<var::IntegerSubOp>);
}

unsigned AstMaker::make_div(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned lineno) {
    return _ast.make_rval<rvals::Binary>(lineno, rval1_idx, rval2_idx,
                                         var::operation<var::IntegerDivOp>);
}

unsigned AstMaker::make_mul(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned lineno) {
    return _ast.make_rval<rvals::Binary>(lineno, rval1_idx, rval2_idx,
                                         var::operation<var::IntegerMulOp>);
}

unsigned AstMaker::make_not(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx, var::not_op);
}

unsigned AstMaker::make_mxtrue(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::mx_operation<var::LogicalTrue>);
}

unsigned AstMaker::make_mxfalse(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::mx_operation<var::LogicalFalse>);
}

unsigned AstMaker::make_mxeq(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::mx_operation<var::IntegerEq>);
}

unsigned AstMaker::make_mxlt(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::mx_operation<var::IntegerLt>);
}

unsigned AstMaker::make_mxgt(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::mx_operation<var::IntegerGt>);
}

unsigned AstMaker::make_mxlte(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::mx_operation<var::IntegerLte>);
}

unsigned AstMaker::make_mxgte(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::mx_operation<var::IntegerGte>);
}

unsigned AstMaker::make_eleq(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::operation<var::IntegerEq>);
}

unsigned AstMaker::make_ellt(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::operation<var::IntegerLt>);
}

unsigned AstMaker::make_elgt(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::operation<var::IntegerGt>);
}

unsigned AstMaker::make_ellte(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::operation<var::IntegerLte>);
}

unsigned AstMaker::make_elgte(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx,
                                        var::operation<var::IntegerGte>);
}

unsigned AstMaker::make_size(unsigned rval_idx, unsigned lineno) {
    return _ast.make_rval<rvals::Unary>(lineno, rval_idx, var::size);
}

unsigned AstMaker::make_ref(std::string_view var_name, unsigned lineno) {
    auto info = _variables_avaliable.get_var_info(var_name);
    if (!info) throw VarUnknown(var_name, lineno);
    return _ast.make_rval<rvals::Ref>(lineno, info->metainf.real_idx);
}

unsigned AstMaker::make_res(std::string_view task_name, unsigned lineno) {
    auto m = _ast.find_task_metainf(task_name);
    if (!m) throw TaskUnknown(task_name, lineno);
    return _ast.make_rval<rvals::Res>(lineno, m->task_idx);
}

unsigned AstMaker::make_env(unsigned lineno) {
    return _ast.make_rval<rvals::Env>(lineno);
}

unsigned AstMaker::make_idx(unsigned rval_idx, std::vector<unsigned> &&dim_list,
                            unsigned lineno) {
    return _ast.make_rval<rvals::Idx>(lineno, rval_idx, std::move(dim_list));
}
}  // namespace ast
