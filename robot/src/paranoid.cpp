#include "robot/exceptions/robot_exceptions.hpp"
#include "robot/robot.hpp"
#include "var/bool.hpp"
#include "var/var_ops.hpp"
#include <random>
#include <robot/paranoid.hpp>

namespace robot {

paranoia_config ParanoiaConfigGenerator::random_paranoia_scores() noexcept {
    return {
        .punnish_rotations = rotations_dist(gen),
        .punnish_env_asks = env_asks_dist(gen),
        .punnish_politeness_score = politeness_score_dist(gen),
        .punnish_impoliteness_score = impoliteness_score_dist(gen),
        .punnish_movements = movements_dist(gen),
        .minimal_happiness = minimal_happiness_dist(gen)
    };
}

Paranoid::Paranoid(Maze &maze, const paranoia_config &cfg) :
    maze(maze), cfg(cfg) {}

Paranoid::Paranoid(Maze &maze, ParanoiaConfigGenerator &gen) :
    maze(maze), cfg(gen.random_paranoia_scores()) {}

void Paranoid::move() {
    if (!maze.move_robot()) throw RobotMoveIntoTheWallError();
    movements_count += 1;
    consequent_rotations = 0;
    consequent_env_asks = 0;

    for (unsigned x = 0; x < Robot::VIEW_DIAMETER; ++x) {
        for (unsigned y = 0; y < Robot::VIEW_DIAMETER; ++y) {
            if (std::get<var::Var<bool_t>>(var::idx(env, {y + 1, x + 1}))[1]) {
                happiness_score = std::max(
                    happiness_score + VIEW_EXIT_HAPPINESS, MAXIMUM_HAPPINESS
                );
                break;
            }
        }
    }

    update_current_env();
}

void Paranoid::rotate_r() {
    maze.rotate_robot_right();
    consequent_rotations += 1;
    update_current_env();
}

void Paranoid::rotate_l() {
    maze.rotate_robot_left();
    consequent_rotations += 1;
    update_current_env();
}

void Paranoid::ask(bool is_politely_asked) {
    if (is_politely_asked) pleasure_ask_score += 1;
    else pleasure_ask_score -= 1;
    update_happiness();
}

void Paranoid::update_current_env() {
    int happiness_range = MAXIMUM_HAPPINESS - cfg.minimal_happiness;
    int happiness_value = happiness_score - cfg.minimal_happiness;
    int view_point_dim = happiness_range / 5;
    int view_radius = (happiness_value + view_point_dim - 1) / view_point_dim;
    Robot::get_env(maze, env, view_radius);
}

var::var_type Paranoid::get_env(bool debug_call) {
    if (!debug_call) consequent_env_asks += 1;
    update_current_env();
    return env;
}

void Paranoid::update_happiness() {
    if (consequent_rotations > cfg.punnish_rotations)
        happiness_score -= consequent_rotations - cfg.punnish_rotations;
    if (consequent_env_asks > cfg.punnish_env_asks)
        happiness_score -= consequent_env_asks - cfg.punnish_env_asks;
    if (pleasure_ask_score > cfg.punnish_politeness_score)
        happiness_score -= pleasure_ask_score - cfg.punnish_politeness_score;
    if (pleasure_ask_score < cfg.punnish_impoliteness_score)
        happiness_score -= cfg.punnish_impoliteness_score - pleasure_ask_score;
    if (movements_count > cfg.punnish_movements)
        happiness_score -= STAY_TOO_LONG_PUNNISH;

    if (happiness_score < cfg.minimal_happiness) throw RobotIsUnhappyError();
}

} // namespace robot
