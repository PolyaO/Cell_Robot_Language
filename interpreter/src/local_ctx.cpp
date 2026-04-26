#include "interpreter/exec/local_ctx.hpp"

#include <stdexcept>

namespace exec {
LocalCtx::LocalCtx(unsigned variables_size, unsigned scope_counters_size) {
    _variables.resize(variables_size);
    _scope_counters.resize(scope_counters_size);
}

var::var_type *LocalCtx::get_var(unsigned idx) {
    if (idx >= _variables.size()) throw std::runtime_error("Bad idx\n");
    return _variables[idx].get();
}

unsigned LocalCtx::get_task_idx() const { return _task_idx; }
unsigned LocalCtx::get_counter(unsigned idx) const {
    if (idx >= _scope_counters.size()) throw std::runtime_error("Bad idx\n");
    return _scope_counters[idx];
}

void LocalCtx::set_task_idx(unsigned idx) { _task_idx = idx; }
void LocalCtx::set_counter(unsigned counter, unsigned idx) {
    _scope_counters[idx] = counter;
}

void LocalCtx::set_var(var::var_type &&v, unsigned idx) {
    _variables[idx].set(std::move(v));
}

void LocalCtx::set_var(var::var_type *v, unsigned idx) {
    _variables[idx].set(v);
}

void LocalCtx::increment_counter(unsigned idx) { _scope_counters[idx]++; }

void LocalCtx::decrement_counter(unsigned idx) { _scope_counters[idx]--; }
}  // namespace exec
