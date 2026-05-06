#include <robot/ideal_robot.hpp>

#include "robot/exceptions/robot_exceptions.hpp"
#include "robot/maze.hpp"
#include "var/var.hpp"

namespace robot {
IdealRobot::IdealRobot(Maze &maze) : maze(maze) {};

var::var_type IdealRobot::get_env() {
    return Robot::get_env(maze, VIEW_RADIUS);
}

void IdealRobot::move() {
    if (!maze.move_robot()) throw RobotMoveIntoTheWallError();
}

void IdealRobot::rotate_r() { maze.rotate_robot_right(); }

void IdealRobot::rotate_l() { maze.rotate_robot_left(); }

void IdealRobot::ask(bool is_politely_asked) { return; }
}  // namespace robot
