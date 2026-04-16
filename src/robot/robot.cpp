#include "backend/rvals/var/var.hpp"
#include "backend/rvals/var/var_ops.hpp"
#include <robot/robot.hpp>

// clang-format off
var::var_type
robot::Robot::get_env(const Maze &maze, unsigned radius) noexcept {
    unsigned diameter = radius * 2 + 1;
    var::var_type env = var::Var<bool_t>(false, {diameter, diameter, 2});

    for (unsigned y = 0; y < diameter; ++y) {
        for (unsigned x = 0; x < diameter; ++x) {
            auto shift_x = maze.get_robot_direction()
                               .get_couner_clockwize()
                               .get_vector()
                               .scale((int)radius - (int)x);

            auto shift_y = maze.get_robot_direction()
                               .get_vector()
                               .scale((int)radius - (int)y);

            auto pos = maze.get_robot_position().add(shift_x).add(shift_y);

            auto env_cell = std::get<var::Var<bool_t>>(
                    var::idx(env, {y + 1, x + 1})
                );
            switch (maze.get_position_state(pos)) {
            case Maze::INVALID:
            case Maze::WALL:  env_cell[0] = true;  env_cell[1] = false; break;
            case Maze::EMPTY: env_cell[0] = false; env_cell[1] = false; break;
            case Maze::EXIT:  env_cell[0] = false; env_cell[1] = true; break;
            }
        }
    }

    return env;
}
