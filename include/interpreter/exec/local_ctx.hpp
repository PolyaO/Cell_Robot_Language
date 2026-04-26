#pragma once

#include "interpreter/exec/var_wrapper.hpp"
#include "var/var.hpp"
namespace exec {
class LocalCtx {
    public:
    LocalCtx(unsigned variables_size, unsigned scope_counters_size);
    var::var_type *get_var(unsigned idx);
    unsigned get_counter(unsigned idx) const;
    unsigned get_task_idx() const;
    void set_task_idx(unsigned idx);
    void set_counter(unsigned counter, unsigned idx);
    void set_var(var::var_type &&v, unsigned idx);
    void set_var(var::var_type *v, unsigned idx);
    void increment_counter(unsigned idx);
    void decrement_counter(unsigned idx);

    private:
    std::vector<VarWrapper> _variables;
    std::vector<unsigned> _scope_counters;
    unsigned _task_idx;
};
}
