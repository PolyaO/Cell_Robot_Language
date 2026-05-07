#include "var/var.hpp"
#include "var/var_ops.hpp"
#include <robot/robot.hpp>

// clang-format off
void robot::Robot::get_env(
        const Maze &maze, var::var_type &env, unsigned radius
) noexcept {
    unsigned diameter = radius * 2 + 1;
    unsigned rng_start = (VIEW_DIAMETER - diameter) / 2;
    unsigned rng_end = VIEW_DIAMETER - rng_start;

    for (unsigned y = 0; y < VIEW_DIAMETER; ++y) {
        for (unsigned x = 0; x < VIEW_DIAMETER; ++x) {
            auto env_cell = std::get<var::Var<bool_t>>(
                    var::idx(env, {y + 1, x + 1})
                );

            if (x < rng_start || x >= rng_end || y < rng_start || y >= rng_end) {
                env_cell[0] = true;
                env_cell[1] = false;
                continue;
            }

            auto shift_x = maze.get_robot_direction()
                               .get_couner_clockwize()
                               .get_vector()
                               .scale((int)radius - (int)x);

            auto shift_y = maze.get_robot_direction()
                               .get_vector()
                               .scale((int)radius - (int)y);

            auto pos = maze.get_robot_position().add(shift_x).add(shift_y);

            switch (maze.get_position_state(pos)) {
            case Maze::INVALID:
            case Maze::WALL:  env_cell[0] = true;  env_cell[1] = false; break;
            case Maze::EMPTY: env_cell[0] = false; env_cell[1] = false; break;
            case Maze::EXIT:  env_cell[0] = false; env_cell[1] = true; break;
            }
        }
    }
}
