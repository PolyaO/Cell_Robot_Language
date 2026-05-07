#include "robot/exceptions/robot_exceptions.hpp"
#include "robot/robot.hpp"
#include "var/bool.hpp"
#include "var/var_ops.hpp"
#include <random>
#include <robot/paranoid.hpp>

namespace robot {

Robot::paranoia_config
ParanoiaConfigGenerator::random_paranoia_scores() noexcept {
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
    stats.movements_count += 1;
    stats.consequent_rotations = 0;
    stats.consequent_env_asks = 0;

    for (unsigned x = 0; x < Robot::VIEW_DIAMETER; ++x) {
        for (unsigned y = 0; y < Robot::VIEW_DIAMETER; ++y) {
            if (std::get<var::Var<bool_t>>(var::idx(env, {y + 1, x + 1}))[1]) {
                stats.happiness_score = std::max(
                    stats.happiness_score + VIEW_EXIT_HAPPINESS,
                    stats.MAXIMUM_HAPPINESS
                );
                break;
            }
        }
    }

    update_current_env();
}

void Paranoid::rotate_r() {
    maze.rotate_robot_right();
    stats.consequent_rotations += 1;
    update_current_env();
}

void Paranoid::rotate_l() {
    maze.rotate_robot_left();
    stats.consequent_rotations += 1;
    update_current_env();
}

void Paranoid::ask(bool is_politely_asked) {
    if (is_politely_asked) stats.pleasure_ask_score += 1;
    else stats.pleasure_ask_score -= 1;
    update_happiness();
}

void Paranoid::update_current_env() {
    int happiness_range = stats.MAXIMUM_HAPPINESS - cfg.minimal_happiness;
    int happiness_value = stats.happiness_score - cfg.minimal_happiness;
    int view_point_dim = happiness_range / 5;
    int view_radius =
        std::min((happiness_value + view_point_dim - 1) / view_point_dim, 5);
    Robot::get_env(maze, env, view_radius);
}

var::var_type Paranoid::get_env(bool debug_call) {
    if (!debug_call) stats.consequent_env_asks += 1;
    update_current_env();
    return env;
}

void Paranoid::update_happiness() {
    if (stats.consequent_rotations > cfg.punnish_rotations)
        stats.happiness_score -=
            stats.consequent_rotations - cfg.punnish_rotations;
    if (stats.consequent_env_asks > cfg.punnish_env_asks)
        stats.happiness_score -=
            stats.consequent_env_asks - cfg.punnish_env_asks;
    if (stats.pleasure_ask_score > cfg.punnish_politeness_score)
        stats.happiness_score -=
            stats.pleasure_ask_score - cfg.punnish_politeness_score;
    if (stats.pleasure_ask_score < cfg.punnish_impoliteness_score)
        stats.happiness_score -=
            cfg.punnish_impoliteness_score - stats.pleasure_ask_score;
    if (stats.movements_count > cfg.punnish_movements)
        stats.happiness_score -= STAY_TOO_LONG_PUNNISH;

    if (stats.happiness_score < cfg.minimal_happiness)
        throw RobotIsUnhappyError();
}

Robot::robot_stats Paranoid::get_stats() const noexcept { return stats; }

Robot::paranoia_config Paranoid::get_paranoia() const noexcept { return cfg; }

} // namespace robot
