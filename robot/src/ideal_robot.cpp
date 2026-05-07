#include <robot/ideal_robot.hpp>

#include "robot/exceptions/robot_exceptions.hpp"
#include "robot/maze.hpp"
#include "var/var.hpp"

namespace robot {
IdealRobot::IdealRobot(Maze &maze) : maze(maze) {};

var::var_type IdealRobot::get_env(bool debug_call) {
    var::var_type env = var::Var<bool_t>(false, {11, 11, 2});
    Robot::get_env(maze, env, 5);
    if (!debug_call) stats.consequent_env_asks += 1;
    return env;
}

void IdealRobot::move() {
    if (!maze.move_robot()) throw RobotMoveIntoTheWallError();
    stats.movements_count += 1;
    stats.consequent_env_asks = 0;
    stats.consequent_rotations = 0;
}

void IdealRobot::rotate_r() {
    maze.rotate_robot_right();
    stats.consequent_rotations += 1;
}

void IdealRobot::rotate_l() {
    maze.rotate_robot_left();
    stats.consequent_rotations += 1;
}

void IdealRobot::ask(bool is_politely_asked) {
    if (is_politely_asked) stats.pleasure_ask_score += 1;
    else stats.pleasure_ask_score -= 1;
}

Robot::robot_stats IdealRobot::get_stats() const noexcept { return stats; }

Robot::paranoia_config IdealRobot::get_paranoia() const noexcept { return cfg; }

} // namespace robot
