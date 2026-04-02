#include <robot/winutil/window_maze.hpp>

#include "robot/maze.hpp"

namespace robot {

Maze &WindowMaze::get_maze() noexcept { return maze; }
const Maze &WindowMaze::get_maze() const noexcept { return maze; }

void WindowMaze::update() {
    static_cast<BasicWindow *>(this)->update();
    draw_robot();
}

void WindowMaze::open(std::string_view filename) {
    maze.read_file(filename);
}

void WindowMaze::draw_maze() {
    auto area = get_area();
    auto desc = area.get_info();

    for (int y = 0; y < maze.get_height(); ++y) {
        if (y >= desc.height) break;
        auto screen_line = area.get_line(y);
        for (int x = 0; x < maze.get_width(); ++x) {
            if (x * 2 + 1 >= desc.width) break;
            char c;
            switch (maze.get_position_state({x, y})) {
                case Maze::EMPTY:
                    c = EMPTY_CHAR;
                    break;
                case Maze::WALL:
                    c = WALL_CHAR;
                    break;
                case Maze::EXIT:
                    c = EXIT_CHAR;
                    break;
                default:
                    c = '?';
                    break;
            }
            screen_line[x * 2].set(c);
            screen_line[x * 2 + 1].set(c);
        }
    }
}

void WindowMaze::draw_robot() {
    auto area = get_area();
    auto desc = area.get_info();

    Maze::position pos = maze.get_robot_position();
    if (pos.x * 2 + 1 >= desc.width || pos.y >= desc.height) return;

    auto screen_line = area.get_line(pos.y);
    screen_line[pos.x * 2].set(ROBOT_CHAR);
    screen_line[pos.x * 2 + 1].set(ROBOT_CHAR);
}

unsigned WindowMaze::get_optimal_width() const noexcept {
    return maze.get_width() * 2;
}

unsigned WindowMaze::get_optimal_height() const noexcept {
    return maze.get_height();
}

}  // namespace robot
