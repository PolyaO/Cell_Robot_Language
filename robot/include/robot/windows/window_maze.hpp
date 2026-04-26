#pragma once
#include <string_view>

#include "robot/maze.hpp"
#include "winutil/windows/base-window.hpp"

namespace robot {

class WindowMaze : public Winutil::BaseWindow {
   public:
    WINDOW_CONSTRUCTOR(WindowMaze)

    void update() override;
    void open(std::string_view filename);

    Maze &get_maze() noexcept;
    const Maze &get_maze() const noexcept;

    void draw_maze();

    unsigned get_optimal_width() const noexcept;
    unsigned get_optimal_height() const noexcept;

   private:
    const static wchar_t WALL_CHAR = L'█';
    const static wchar_t EMPTY_CHAR = L' ';
    const static wchar_t EXIT_CHAR = L'𜺱';
    const static wchar_t ROBOT_CHAR = L'𜷰';

    void draw_robot();
    void set_wall(Maze::position pos);
    void set_exit(Maze::position pos);
    void set_empty(Maze::position pos);

    Maze maze;
    Maze::position robot_pos;
};

};  // namespace robot

