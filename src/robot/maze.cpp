#include "robot/exceptions/maze_exceptions.hpp"
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <robot/maze.hpp>
#include <string>

namespace robot {

unsigned Maze::maze_t::index_of(position pos) const noexcept {
    return pos.y * width + pos.x;
}

bool Maze::maze_t::is_wall(position pos) const {
    return is_valid(pos) && maze.at(index_of(pos));
}

bool Maze::maze_t::is_empty(position pos) const {
    return is_valid(pos) && !maze.at(index_of(pos));
}

bool Maze::maze_t::is_exit(position pos) const {
    return is_empty(pos)
        && (pos.x == 0 || pos.y == 0 || pos.x == width - 1
            || pos.y == height - 1);
}

bool Maze::maze_t::is_valid(position pos) const noexcept {
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

Maze::direction Maze::direction::random() noexcept {
    return direction(rand() % 4);
}

Maze::direction Maze::direction::get_clockwize() const noexcept {
    return direction((dir + 1) % 4);
}

Maze::direction Maze::direction::get_opposite() const noexcept {
    return direction((dir + 2) % 4);
}

Maze::direction Maze::direction::get_couner_clockwize() const noexcept {
    return direction(dir == 0 ? 3 : dir - 1);
}

int Maze::direction::get_x() const noexcept {
    switch (dir) {
    case LEFT: return -1;
    case RIGHT: return 1;
    default: return 0;
    }
}

int Maze::direction::get_y() const noexcept {
    switch (dir) {
    case UP: return -1;
    case DOWN: return 1;
    default: return 0;
    }
}

Maze::vector2 Maze::direction::get_vector() const noexcept {
    return {.x = get_x(), .y = get_y()};
}

Maze::vector2 Maze::vector2::relative(direction dir) const noexcept {
    return {.x = x + dir.get_x(), .y = y + dir.get_y()};
}

Maze::position Maze::position::add(position other) const noexcept {
    return {.x = x + other.x, .y = y + other.y};
}

Maze::position Maze::position::add(int x, int y) const noexcept {
    return {.x = this->x + x, .y = this->y + y};
}

Maze::position Maze::position::sub(vector2 other) const noexcept {
    return {.x = x - other.x, .y = y - other.y};
}
Maze::position Maze::position::sub(int x, int y) const noexcept {
    return {.x = this->x - x, .y = this->y - y};
}

Maze::vector2 Maze::position::scale(int scalar) const noexcept {
    return {.x = x * scalar, .y = y * scalar};
}

Maze::vector2 Maze::position::scale(double scalar) const noexcept {
    return {.x = (int)(x * scalar), .y = (int)(y * scalar)};
}

void Maze::read_file(std::string_view filename) {
    std::ifstream in;
    in.open(filename.data());

    std::string line;
    int x = 0, y = 0;
    while (std::getline(in, line)) {
        maze.add_line(
            std::ranges::transform_view(line, [this, &x, y](char c) {
                if (c == ROBOT_CHAR) {
                    this->robot_pos = {x, y};
                    this->robot_dir = direction::random();
                }
                x += 1;
                return c == WALL_CHAR;
            })
        );
        x = 0;
        y += 1;
    }

    in.close();

    if (maze.height < MINIMAL_MAZE_HEIGHT)
        throw MazeLoadInvalidHeightError(maze.height, MINIMAL_MAZE_HEIGHT);

    if (robot_pos == (position){}) place_robot();
}

void Maze::place_robot() {
    int row = (rand() % (maze.height - 1)) + 1;
    int col = (rand() % (maze.width - 1)) + 1;

    robot_pos = {row, col};
    if (maze.is_empty(robot_pos)) return;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            position new_pos = robot_pos.add(x, y);
            if (maze.is_empty(new_pos)) {
                robot_pos = new_pos;
                return;
            }
        }
    }
    robot_dir = direction::random();
}

bool Maze::move_robot() noexcept {
    position new_pos = robot_pos.relative(robot_dir);
    if (maze.is_valid(new_pos) && !maze.is_wall(new_pos)) {
        robot_pos = new_pos;
        return true;
    }
    return false;
}

void Maze::rotate_robot_left() noexcept {
    robot_dir = robot_dir.get_couner_clockwize();
}

void Maze::rotate_robot_right() noexcept {
    robot_dir = robot_dir.get_clockwize();
}

Maze::cell_state Maze::get_position_state(position pos) const noexcept {
    if (!maze.is_valid(pos)) return INVALID;
    if (maze.is_exit(pos)) return EXIT;
    if (maze.is_wall(pos)) return WALL;
    return EMPTY;
}

unsigned Maze::get_width() const noexcept { return maze.width; }

unsigned Maze::get_height() const noexcept { return maze.height; }

} // namespace robot
