#pragma once

#include <list>

#include "interpreter/ast.hpp"
#include "interpreter/exec/local_ctx.hpp"
namespace exec {
class GlobalCtx {
   public:
    GlobalCtx() = default;
    LocalCtx &get_curr_ctx();
    void push_curr_ctx(LocalCtx &&);
    void pop_curr_ctx();

    ast::Ast ast;
   private:
    std::list<LocalCtx> ctx_stack;
};

}  // namespace exec
