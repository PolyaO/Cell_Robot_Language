#include "interpreter/exec/global_ctx.hpp"

namespace exec {
LocalCtx &GlobalCtx::get_curr_ctx() { return ctx_stack.back(); }
void GlobalCtx::push_curr_ctx(LocalCtx &&ctx) { ctx_stack.emplace_back(ctx); }
void GlobalCtx::pop_curr_ctx() { ctx_stack.pop_back(); }
}  // namespace exec
