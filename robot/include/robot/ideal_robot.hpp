#pragma once

#include "robot/maze.hpp"
#include "robot/robot.hpp"

namespace robot {

class IdealRobot : public Robot {
   public:
    IdealRobot(Maze &maze);
    ~IdealRobot() override = default;
    var::var_type get_env() override;
    void move() override;
    void rotate_r() override;
    void rotate_l() override;
    void ask(bool is_politely_asked = false) override;

   private:
    Maze &maze;
    constexpr const static int VIEW_RADIUS = 5;
};

}  // namespace robot
