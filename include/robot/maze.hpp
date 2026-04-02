#pragma once
#include "robot/exceptions/maze_exceptions.hpp"
#include <ranges>
#include <string_view>
#include <vector>

namespace robot {

class Maze {
  public:
    const static char WALL_CHAR = '#';
    const static char EMPTY_CHAR = ' ';
    const static unsigned MINIMAL_MAZE_WIDTH = 5;
    const static unsigned MINIMAL_MAZE_HEIGHT = 5;

    struct vector2;
    using position = vector2;

    struct direction {
        enum { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };
        int dir = UP;

        static direction random() noexcept;

        direction get_clockwize() const noexcept;
        direction get_couner_clockwize() const noexcept;

        int get_x() const noexcept;
        int get_y() const noexcept;

        vector2 get_vector() const noexcept;
    };

    struct vector2 {
        int x;
        int y;

        position relative(direction) const noexcept;
        position add(vector2) const noexcept;
        position add(int x, int y) const noexcept;

        position sub(vector2) const noexcept;
        position sub(int x, int y) const noexcept;

        vector2 scale(int scalar) const noexcept;
        vector2 scale(double scalar) const noexcept;
    };

    enum cell_state { WALL, EMPTY, EXIT, INVALID };

    position get_robot_position() const noexcept { return robot_pos; }
    direction get_robot_direction() const noexcept { return robot_dir; }

    cell_state get_position_state(position) const noexcept;

    unsigned get_width() const noexcept;
    unsigned get_height() const noexcept;

    bool move_robot() noexcept;
    void rotate_robot_left() noexcept;
    void rotate_robot_right() noexcept;

    void read_file(std::string_view filename);

  private:
    struct maze_t {
        std::vector<bool> maze;
        unsigned width = 0;
        unsigned height = 0;

        void add_line(std::ranges::input_range auto &&line);

        bool is_wall(position) const;
        bool is_empty(position) const;
        bool is_exit(position) const;
        bool is_valid(position) const noexcept;

        unsigned index_of(position) const noexcept;
    };
    void place_robot();

    maze_t maze;
    position robot_pos;
    direction robot_dir;
};

void Maze::maze_t::add_line(std::ranges::input_range auto &&line) {
    if (maze.empty()) {
        if (line.size() < MINIMAL_MAZE_WIDTH)
            throw MazeLoadInvalidWidthError(line.size(), MINIMAL_MAZE_WIDTH);
        width = line.size();
    } else if (line.empty() || width != line.size())
        throw MazeLoadNewLineLengthError(height + 1, line.size(), width);
    maze.append_range(line);
    height += 1;
}

}; // namespace robot
