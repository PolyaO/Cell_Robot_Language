#include "backend/rvals/rval.hpp"
#include "backend/rvals/var/var.hpp"
#include "robot/exceptions/robot_exceptions.hpp"
#include "robot/maze.hpp"
#include <robot/ideal_robot.hpp>

namespace robot {
IdealRobot::IdealRobot(Maze &maze) : maze(maze) {};

var::var_type IdealRobot::get_env(bool is_politely_asked) {
    return Robot::get_env(maze, VIEW_RADIUS);
}

void IdealRobot::move(bool is_politely_asked) {
    if (!maze.move_robot()) throw RobotMoveIntoTheWallError();
}

void IdealRobot::rotate_r(bool is_politely_asked) { maze.rotate_robot_right(); }

void IdealRobot::rotate_l(bool is_politely_asked) { maze.rotate_robot_left(); }
} // namespace robot
