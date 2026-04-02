#include "backend/rvals/var/var_ops.hpp"
#include <robot/robot.hpp>

var::var_type
robot::Robot::get_env(const Maze &maze, unsigned radius) noexcept {
    const int diameter = radius * 2 + 1;
    var::dim_t res_dims(3), wall_idx(3), exit_idx(3);

    res_dims[0] = res_dims[1] = diameter;
    res_dims[2] = 2;

    var::var_type env = var::Var<bool_t>(false, std::move(res_dims));

    wall_idx[2] = 1;
    exit_idx[2] = 2;

    for (int x = 0; x < diameter; ++x) {
        wall_idx[0] = exit_idx[0] = x + 1;
        for (int y = 0; y < diameter; ++y) {
            wall_idx[1] = exit_idx[1] = y + 1;

            Maze::position pos =
                maze.get_robot_position().add(x - radius, y - radius);

            var::var_type walls = var::idx(env, wall_idx);
            var::var_type exits = var::idx(env, exit_idx);

            switch (maze.get_position_state(pos)) {
            case Maze::INVALID:
            case Maze::WALL:
                var::assign(walls, var::TRUE);
                var::assign(exits, var::FALSE);
                break;
            case Maze::EMPTY:
                var::assign(walls, var::FALSE);
                var::assign(exits, var::FALSE);
                break;
            case Maze::EXIT:
                var::assign(walls, var::FALSE);
                var::assign(exits, var::TRUE);
                break;
            }
        }
    }

    return env;
}
