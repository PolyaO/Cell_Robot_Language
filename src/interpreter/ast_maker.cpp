#include "interpreter/ast_maker.hpp"

#include <algorithm>
#include <variant>

#include "interpreter/ast.hpp"
#include "interpreter/exceptions/build_exceptions.hpp"
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
    std::visit(
        [](auto &e) {
            if constexpr (requires { e.set_politely_asked(); }) {
                e.set_politely_asked();
            }
        },
        *_ast.get_expr(expr));
}

std::variant<unsigned, std::vector<unsigned>> AstMaker::make_arg_list() const {
    if (_variables_avaliable.empty()) return 0u;
    return std::vector<unsigned>{};
}

void AstMaker::add_to_arg_list(
    std::variant<unsigned, std::vector<unsigned>> &list, std::string_view arg,
    unsigned line) {
    if (std::holds_alternative<unsigned>(list)) {
        if (_variables_avaliable.get_var_info(arg))
            throw ArgNameRepeat(arg, line);
        unsigned ref_idx = _ast.make_rval<Ref>(_vars_number, line);
        Ast::VarMetainf m(std::string(arg), ref_idx, _vars_number);
        VarInfo inf(std::move(m), FAKE_DECLARE_IDX, line);
        _variables_avaliable.add_var_info(std::move(inf));
        std::get<unsigned>(list)++;
        _vars_number++;
    } else {
        auto arg_inf_ptr = _variables_avaliable.get_var_info(arg);
        if (!arg_inf_ptr) throw ArgNameUnknown(arg, line);
        std::get<1>(list).emplace_back(arg_inf_ptr->metainf.real_idx);
    }
}

std::vector<unsigned> AstMaker::make_dim_list() const {
    return std::vector<unsigned>{};
}

void AstMaker::add_to_dim_list(std::vector<unsigned> &list, int dim,
                               unsigned line) const {
    if (dim <= 0) throw WrongDim(dim, line);
    list.emplace_back(dim);
}

std::vector<unsigned> AstMaker::make_stmts() const {
    return std::vector<unsigned>{};
}

void AstMaker::add_to_stmts(std::vector<unsigned> &stmts,
                            unsigned expr_) const {
    stmts.emplace_back(expr_);
}

unsigned AstMaker::make_task(
    std::string_view task_name,
    const std::variant<unsigned, std::vector<unsigned>> &arg_list,
    std::vector<unsigned> &&exprs, std::string_view res, unsigned line) {
    auto var_info_ptr = _variables_avaliable.get_var_info(res);
    if (!var_info_ptr) throw ResNameUnknown(res, task_name, line);
    return make_task(task_name, std::move(exprs), std::get<unsigned>(arg_list),
                     var_info_ptr->metainf.ref_idx, line);
}

unsigned AstMaker::make_task(std::string_view task_name,
                             std::vector<unsigned> &&exprs, unsigned args_num,
                             unsigned res_ref_idx, unsigned line) {
    auto task_inf_ptr = _ast.find_task_metainf(task_name);
    if (task_inf_ptr)
        throw TaskRedeclare(task_name, task_inf_ptr->decl_line, line);
    Ast::TaskMetainf m;
    unsigned task_idx = m.task_idx =
        _ast.make_expr<Task>(std::move(exprs), _counters_number, line);
    m.task_name = task_name;
    m.decl_line = line;
    m.args_number = args_num;
    m.res_ref_idx = res_ref_idx;
    m.ctx_vars_number = _vars_number;
    m.ctx_scope_counters_number = ++_counters_number;
    _task_vars.append_range(_variables_avaliable.clear());
    m.vars_metainf = std::move(_task_vars);
    _vars_number = 0;
    _counters_number = 0;
    _ast.add_task_metainf(std::move(m));
    process_task_calls(task_name);
    return task_idx;
}

unsigned AstMaker::make_findexit(std::vector<unsigned> &&exprs, unsigned line) {
    return make_task("FINDEXIT", std::move(exprs), 0, 0, line);
}

void AstMaker::process_task_calls(std::string_view task_name) {
    for (auto &call : _unprocessed_task_calls) {
        if (call.task_name != task_name) {
            throw TaskUnknown(call.task_name, call.line);
        } else {
            Do &do_ = std::get<Do>(*_ast.get_expr(call.do_idx));
            do_.set_task_idx(_ast.find_task_metainf(task_name)->task_idx);
        }
    }
    _unprocessed_task_calls.clear();
}

unsigned AstMaker::make_assignement(std::string_view var_name,
                                    std::vector<unsigned> &&dim_list,
                                    unsigned rval_idx, unsigned line) {
    auto var_info_ptr = _variables_avaliable.get_var_info(var_name);
    if (!var_info_ptr) throw VarUnknown(var_name, line);
    unsigned idx = var_info_ptr->metainf.ref_idx;
    if (!dim_list.empty()) {
        idx = _ast.make_rval<Idx>(idx, std::move(dim_list), line);
    }
    return _ast.make_expr<Assign>(idx, rval_idx, line);
}

unsigned AstMaker::make_do(
    std::string_view task_name,
    std::variant<unsigned, std::vector<unsigned>> &&arg_list, unsigned line) {
    unsigned idx = 0;
    auto m_ptr = _ast.find_task_metainf(task_name);
    if (m_ptr) idx = m_ptr->task_idx;
    auto do_idx =
        _ast.make_expr<Do>(idx, std::move(std::get<1>(arg_list)), line);
    if (!idx) {
        _unprocessed_task_calls.emplace_back(
            TaskCallInfo(do_idx, line, std::string(task_name)));
    }
    return do_idx;
}

unsigned AstMaker::make_for(std::string_view counter, std::string_view boundary,
                            std::string_view step, unsigned stmt,
                            unsigned line) {
    auto counter_inf_ptr = _variables_avaliable.get_var_info(counter);
    if (!counter_inf_ptr) throw VarUnknown(counter, line);
    auto boundary_inf_ptr = _variables_avaliable.get_var_info(boundary);
    if (!boundary_inf_ptr) throw VarUnknown(boundary, line);
    auto step_inf_ptr = _variables_avaliable.get_var_info(step);
    if (!step_inf_ptr) throw VarUnknown(step, line);

    auto res = _ast.make_expr<For>(counter_inf_ptr->metainf.real_idx,
                               boundary_inf_ptr->metainf.real_idx,
                               step_inf_ptr->metainf.real_idx, stmt,  _vars_number++,
                               _counters_number, _counters_number+1, line);
    _counters_number += 2;
    return res;
}

unsigned AstMaker::make_switch(unsigned rval_idx, bool condition1,
                               unsigned stmt1, bool condition2, unsigned stmt2,
                               unsigned line) {
    if (condition1) {
        if (condition2) throw DoubleLogicLiteral("TRUE", line);
        return _ast.make_expr<Switch>(rval_idx, stmt1, stmt2, line);
    }
    if (!condition2 && stmt2) throw DoubleLogicLiteral("FALSE", line);
    return _ast.make_expr<Switch>(rval_idx, stmt2, stmt1, line);
}

unsigned AstMaker::make_scope(std::vector<unsigned> &&exprs, unsigned line) {
    _variables_avaliable.remove_declared(exprs, _task_vars);
    return _ast.make_expr<Scope>(std::move(exprs), _counters_number++, line);
}

unsigned AstMaker::make_and(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::LogicalAnd>, line);
}

unsigned AstMaker::make_or(unsigned rval1_idx, unsigned rval2_idx,
                           unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::LogicalOr>, line);
}

unsigned AstMaker::make_sum(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::IntegerSumOp>, line);
}

unsigned AstMaker::make_sub(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::IntegerSubOp>, line);
}

unsigned AstMaker::make_div(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::IntegerDivOp>, line);
}

unsigned AstMaker::make_mul(unsigned rval1_idx, unsigned rval2_idx,
                            unsigned line) {
    return _ast.make_rval<Binary>(rval1_idx, rval2_idx,
                                  var::operation<var::IntegerMulOp>, line);
}

unsigned AstMaker::make_not(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::not_op, line);
}

unsigned AstMaker::make_mxtrue(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::LogicalTrue>,
                                 line);
}

unsigned AstMaker::make_mxfalse(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::LogicalFalse>,
                                 line);
}

unsigned AstMaker::make_mxeq(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerEq>,
                                 line);
}

unsigned AstMaker::make_mxlt(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerLt>,
                                 line);
}

unsigned AstMaker::make_mxgt(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerGt>,
                                 line);
}

unsigned AstMaker::make_mxlte(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerLte>,
                                 line);
}

unsigned AstMaker::make_mxgte(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::mx_operation<var::IntegerGte>,
                                 line);
}

unsigned AstMaker::make_eleq(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerEq>,
                                 line);
}

unsigned AstMaker::make_ellt(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerLt>,
                                 line);
}

unsigned AstMaker::make_elgt(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerGt>,
                                 line);
}

unsigned AstMaker::make_ellte(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerLte>,
                                 line);
}

unsigned AstMaker::make_elgte(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::operation<var::IntegerGte>,
                                 line);
}

unsigned AstMaker::make_size(unsigned rval_idx, unsigned line) {
    return _ast.make_rval<Unary>(rval_idx, var::size, line);
}

unsigned AstMaker::make_ref(std::string_view var_name, unsigned line) {
    auto info = _variables_avaliable.get_var_info(var_name);
    if (!info) throw VarUnknown(var_name, line);
    return _ast.make_rval<Ref>(info->metainf.real_idx, line);
}

unsigned AstMaker::make_res(std::string_view task_name, unsigned line) {
    auto m = _ast.find_task_metainf(task_name);
    if (!m) throw TaskUnknown(task_name, line);
    return _ast.make_rval<Res>(m->task_idx, line);
}

unsigned AstMaker::make_env(unsigned line) { return _ast.make_rval<Env>(line); }

unsigned AstMaker::make_idx(unsigned rval_idx, std::vector<unsigned> &&dim_list,
                            unsigned line) {
    return _ast.make_rval<Idx>(rval_idx, std::move(dim_list), line);
}
}  // namespace ast
