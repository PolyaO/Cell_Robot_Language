#pragma once
#include "interpreter/ast.hpp"
#include "robot/robot.hpp"
#include <memory>

namespace ast {
struct ExecCtx {
    Ast ast;
    unsigned curr_task_idx;
    std::unique_ptr<robot::Robot> robot;
};

}  // namespace ast
