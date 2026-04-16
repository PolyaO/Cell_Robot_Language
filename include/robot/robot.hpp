#pragma once
#include "backend/rvals/var/var.hpp"
#include "robot/maze.hpp"

namespace robot {

class Robot {
  public:
    virtual var::var_type get_env(bool is_politely_asked = false) = 0;
    virtual void move(bool is_politely_asked = false) = 0;
    virtual void rotate_r(bool is_politely_asked = false) = 0;
    virtual void rotate_l(bool is_politely_asked = false) = 0;
  protected:
    static var::var_type get_env(const Maze &maze, unsigned radius) noexcept;
};

} // namespace robot
