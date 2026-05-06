#pragma once
#include "robot/maze.hpp"
#include "var/var.hpp"

namespace robot {

class Robot {
  public:
    virtual ~Robot() = default;
    virtual var::var_type get_env(bool debug_call = false) = 0;
    virtual void move() = 0;
    virtual void rotate_r() = 0;
    virtual void rotate_l() = 0;
    virtual void ask(bool is_politely_asked = false) = 0;

    constexpr const static unsigned VIEW_DIAMETER = 11;

  protected:
    static void
    get_env(const Maze &maze, var::var_type &env, unsigned radius) noexcept;
};

} // namespace robot
