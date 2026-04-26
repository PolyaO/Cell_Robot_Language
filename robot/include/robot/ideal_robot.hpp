#pragma once

#include "robot/maze.hpp"
#include "robot/robot.hpp"

namespace robot {

class IdealRobot : Robot {
  public:
    IdealRobot(Maze &maze);
    var::var_type get_env(bool is_politely_asked = false) override;
    void move(bool is_politely_asked = false) override;
    void rotate_r(bool is_politely_asked = false) override;
    void rotate_l(bool is_politely_asked = false) override;

  private:
    Maze &maze;
    const static int VIEW_RADIUS = 5;
};

} // namespace robot
