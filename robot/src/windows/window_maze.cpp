#include "robot/windows/window_maze.hpp"
#include "robot/maze.hpp"
#include "winutil/engine/colors.hpp"

namespace robot {

Maze &WindowMaze::get_maze() noexcept { return maze; }
const Maze &WindowMaze::get_maze() const noexcept { return maze; }

void WindowMaze::update() {
    BaseWindow::update();
    draw_robot();
}

void WindowMaze::open(std::string_view filename) { maze.read_file(filename); }

void WindowMaze::draw_maze() {
    auto area = get_area();
    auto desc = area.get_info();

    for (int y = 0; y < maze.get_height(); ++y) {
        if (y >= desc.height) break;
        for (int x = 0; x < maze.get_width(); ++x) {
            if (x * 2 + 1 >= desc.width) break;
            switch (maze.get_position_state({x, y})) {
            case Maze::WALL: set_wall({x, y}); break;
            case Maze::EXIT: set_exit({x, y}); break;
            default: set_empty({x, y}); break;
            }
        }
    }

    robot_pos = maze.get_robot_position();
}

void WindowMaze::set_empty(Maze::position pos) {
    auto area = get_area();
    auto desc = area.get_info();

    if (pos.x * 2 + 1 >= desc.width || pos.y >= desc.height) return;

    auto screen_line = area.get_line(pos.y);

    screen_line[pos.x * 2].set_char(EMPTY_CHAR);
    screen_line[pos.x * 2 + 1].set_char(EMPTY_CHAR);
}

void WindowMaze::set_exit(Maze::position pos) {
    auto area = get_area();
    auto desc = area.get_info();

    if (pos.x * 2 + 1 >= desc.width || pos.y >= desc.height) return;

    auto screen_line = area.get_line(pos.y);

    screen_line[pos.x * 2].set_char(EXIT_CHAR);
    screen_line[pos.x * 2 + 1].set_char(EXIT_CHAR);
}

void WindowMaze::set_wall(Maze::position pos) {
    auto area = get_area();
    auto desc = area.get_info();

    if (pos.x * 2 + 1 >= desc.width || pos.y >= desc.height) return;

    auto screen_line = area.get_line(pos.y);

    bool wall_up = maze.is_wall(pos.relative(Maze::direction::UP));
    bool wall_down = maze.is_wall(pos.relative(Maze::direction::DOWN));
    bool wall_left = maze.is_wall(pos.relative(Maze::direction::LEFT));
    bool wall_right = maze.is_wall(pos.relative(Maze::direction::RIGHT));
    if (wall_up && wall_down && wall_left && wall_right) {
        screen_line[pos.x * 2].set_char(L'𜶪');
        screen_line[pos.x * 2 + 1].set_char(L'𜵮');
    } else if (wall_up && wall_down && wall_left) {
        screen_line[pos.x * 2].set_char(L'𜶪');
        screen_line[pos.x * 2 + 1].set_char(L'▌');
    } else if (wall_up && wall_down && wall_right) {
        screen_line[pos.x * 2].set_char(L'▐');
        screen_line[pos.x * 2 + 1].set_char(L'𜵮');
    } else if (wall_up && wall_left && wall_right) {
        screen_line[pos.x * 2].set_char(L'𜴵');
        screen_line[pos.x * 2 + 1].set_char(L'𜴴');
    } else if (wall_down && wall_left && wall_right) {
        screen_line[pos.x * 2].set_char(L'𜶨');
        screen_line[pos.x * 2 + 1].set_char(L'𜵭');
    } else if (wall_up && wall_down) {
        screen_line[pos.x * 2].set_char(L'▐');
        screen_line[pos.x * 2 + 1].set_char(L'▌');
    } else if (wall_left && wall_right) {
        screen_line[pos.x * 2].set_char(L'𜴳');
        screen_line[pos.x * 2 + 1].set_char(L'𜴳');
    } else if (wall_up && wall_right) {
        screen_line[pos.x * 2].set_char(L'𜴡');
        screen_line[pos.x * 2 + 1].set_char(L'𜴴');
    } else if (wall_up && wall_left) {
        screen_line[pos.x * 2].set_char(L'𜴵');
        screen_line[pos.x * 2 + 1].set_char(L'𜴍');
    } else if (wall_down && wall_right) {
        screen_line[pos.x * 2].set_char(L'𜶖');
        screen_line[pos.x * 2 + 1].set_char(L'𜵭');
    } else if (wall_down && wall_left) {
        screen_line[pos.x * 2].set_char(L'𜶨');
        screen_line[pos.x * 2 + 1].set_char(L'𜵈');
    } else if (wall_up) {
        screen_line[pos.x * 2].set_char(L'𜴡');
        screen_line[pos.x * 2 + 1].set_char(L'𜴍');
    } else if (wall_down) {
        screen_line[pos.x * 2].set_char(L'𜶖');
        screen_line[pos.x * 2 + 1].set_char(L'𜵈');
    } else if (wall_right) {
        screen_line[pos.x * 2].set_char(L' ');
        screen_line[pos.x * 2 + 1].set_char(L'𜴳');
    } else if (wall_left) {
        screen_line[pos.x * 2].set_char(L'𜴳');
        screen_line[pos.x * 2 + 1].set_char(L' ');
    }
    screen_line[pos.x * 2].set_color(COLOR_FG(GREEN));
    screen_line[pos.x * 2 + 1].set_color(COLOR_FG(GREEN));
}

void WindowMaze::draw_robot() {
    auto area = get_area();
    auto desc = area.get_info();

    auto screen_line = area.get_line(robot_pos.y);

    if (maze.is_exit(robot_pos)) set_exit(robot_pos);
    else set_empty(robot_pos);

    robot_pos = maze.get_robot_position();
    if (robot_pos.x * 2 + 1 >= desc.width || robot_pos.y >= desc.height) return;

    screen_line = area.get_line(robot_pos.y);

    switch ((Maze::direction::dir_e)maze.get_robot_direction()) {
    case Maze::direction::UP:
        screen_line[robot_pos.x * 2].set(L'𜸉');
        screen_line[robot_pos.x * 2 + 1].set(ROBOT_CHAR);
        break;
    case Maze::direction::DOWN:
        screen_line[robot_pos.x * 2].set(ROBOT_CHAR);
        screen_line[robot_pos.x * 2 + 1].set(L'𜸊');
        break;
    case Maze::direction::RIGHT:
        screen_line[robot_pos.x * 2].set(ROBOT_CHAR);
        screen_line[robot_pos.x * 2 + 1].set(L'>');
        break;
    case Maze::direction::LEFT:
        screen_line[robot_pos.x * 2].set(L'<');
        screen_line[robot_pos.x * 2 + 1].set(ROBOT_CHAR);
        break;
    default: break;
    }
}

unsigned WindowMaze::get_optimal_width() const noexcept {
    return maze.get_width() * 2;
}

unsigned WindowMaze::get_optimal_height() const noexcept {
    return maze.get_height();
}

} // namespace robot
