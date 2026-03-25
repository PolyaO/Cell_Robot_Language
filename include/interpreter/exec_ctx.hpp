#pragma once
#include "interpreter/ast.hpp"
#include "robot/robot.hpp"
namespace ast {
struct ExecCtx {
    Ast ast;
    unsigned curr_task_idx;
    robot::Robot robot;
};
}  // namespace ast
