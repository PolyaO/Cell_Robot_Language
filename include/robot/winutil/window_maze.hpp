#pragma once
#include <string_view>

#include "robot/maze.hpp"
#include "winutil/basic-window.hpp"

namespace robot {

class WindowMaze : public Winutil::BasicWindow {
   public:
    WindowMaze(Winutil::engine::DrawArea &&area)
        : BasicWindow(std::move(area)) {}

    void update() override;
    void open(std::string_view filename);

    Maze &get_maze() noexcept;
    const Maze &get_maze() const noexcept;

    void draw_maze();

    unsigned get_optimal_width() const noexcept;
    unsigned get_optimal_height() const noexcept;

   private:
    const static char WALL_CHAR = Maze::WALL_CHAR;
    const static char EMPTY_CHAR = Maze::EMPTY_CHAR;
    const static char EXIT_CHAR = ' ';
    const static char ROBOT_CHAR = '*';

    void draw_robot();

    Maze maze;
};

};  // namespace robot

