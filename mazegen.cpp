#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <string>

#include <robot/maze.hpp>

const int EXIT_FACTOR = 50;

using namespace robot;
using Pos = Maze::position;
using Dir = Maze::direction;

const char PATH = Maze::EMPTY_CHAR;
const char WALL = Maze::WALL_CHAR;
const char ROBOT = Maze::ROBOT_CHAR;

namespace std {
std::string to_string(Maze::position pos) {
    return "pos(" + std::to_string(pos.x) + "," + std::to_string(pos.y) + ")";
}
} // namespace std

class MazeGen {
  private:
    int width, height;
    std::string maze;
    std::mt19937 rng;

  public:
    MazeGen(int w, int h) : rng(std::random_device{}()) {
        if (w % 2 == 0) w++;
        if (h % 2 == 0) h++;
        width = w;
        height = h;
        maze.resize((width + 1) * height, WALL);
        for (int i = 0; i < height; ++i) set_char({width, i}, '\n');
    }

    std::string_view str() const { return maze; }

    Pos random_pos() {
        std::uniform_int_distribution<int> dist_x(0, width / 2 - 1);
        std::uniform_int_distribution<int> dist_y(0, height / 2 - 1);
        return Pos(dist_x(rng) * 2 + 1, dist_y(rng) * 2 + 1);
    }

    bool is_valid_pos(const Pos &pos) const {
        return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
    }

    bool is_wall(const Pos &pos) const { return get_char(pos) == WALL; }

    bool is_path(const Pos &pos) const { return get_char(pos) == PATH; }

    char get_char(const Pos &pos) const {
        return maze[pos.x + pos.y * (width + 1)];
    }

    void set_char(const Pos &pos, char c) {
        maze[pos.x + pos.y * (width + 1)] = c;
    }

    bool has_exits() const {
        for (int i = 0; i < width; i++) {
            if (is_path(Pos(i, 0)) || is_path(Pos(i, height - 1))) {
                return true;
            }
        }
        for (int i = 0; i < height; i++) {
            if (is_path(Pos(0, i)) || is_path(Pos(width - 1, i))) {
                return true;
            }
        }
        return false;
    }

    void carve(const Pos &start_pos) {
        set_char(start_pos, PATH);

        std::array<Dir, 4> dirs = {Dir::UP, Dir::DOWN, Dir::LEFT, Dir::RIGHT};
        std::shuffle(dirs.begin(), dirs.end(), rng);

        std::uniform_int_distribution<int> dist(0, 1000);

        for (Dir dir : dirs) {
            Maze::vector2 dir_vec = dir.get_vector();
            Pos wall_pos = start_pos.add(dir_vec);
            Pos next_pos = wall_pos.add(dir_vec);
            if (is_valid_pos(next_pos)) {
                if (is_wall(next_pos)) {
                    set_char(wall_pos, PATH);
                    carve(next_pos);
                }
            } else if (dist(rng) < EXIT_FACTOR) {
                set_char(wall_pos, PATH);
            }
        }
    }
};

MazeGen get_maze(int width, int height) {
    MazeGen res(width, height);
    res.carve(res.random_pos());
    res.set_char(res.random_pos(), ROBOT);
    return res;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "USAGE: " << argv[0] << " <width> <height>\n";
        return 1;
    }

    int width = std::stoi(argv[1]);
    int height = std::stoi(argv[2]);

    while (true) {
        MazeGen maze = get_maze(width, height);
        if (maze.has_exits()) {
            std::cout << maze.str();
            return 0;
        }
    }

    return 0;
}
