#pragma once

#include "robot/maze.hpp"
#include "robot/robot.hpp"

namespace robot {

class IdealRobot : public Robot {
   public:
    IdealRobot(Maze &maze);
    ~IdealRobot() override = default;
    var::var_type get_env(bool debug_call = false) override;
    void move() override;
    void rotate_r() override;
    void rotate_l() override;
    void ask(bool is_politely_asked = false) override;
    robot_stats get_stats() const noexcept override;
    paranoia_config get_paranoia() const noexcept override;

   private:
    Maze &maze;
    robot_stats stats = { };
    paranoia_config cfg = { };
};

}  // namespace robot
