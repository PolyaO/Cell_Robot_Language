#include <robot/ideal_robot.hpp>

#include "robot/exceptions/robot_exceptions.hpp"
#include "robot/maze.hpp"
#include "var/var.hpp"

namespace robot {
IdealRobot::IdealRobot(Maze &maze) : maze(maze) {};

var::var_type IdealRobot::get_env(bool debug_call) {
    var::var_type env = var::Var<bool_t>(false, {11, 11, 2});
    Robot::get_env(maze, env, 5);
    return env;
}

void IdealRobot::move() {
    if (!maze.move_robot()) throw RobotMoveIntoTheWallError();
}

void IdealRobot::rotate_r() { maze.rotate_robot_right(); }

void IdealRobot::rotate_l() { maze.rotate_robot_left(); }

void IdealRobot::ask(bool is_politely_asked) { return; }
}  // namespace robot
