#include <clocale>
#include <csignal>
#include <cstdio>

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

#undef COLOR
#undef CTRL

// clang-format off
#include "robot/winutil/window_maze.hpp"
#include "robot/ideal_robot.hpp"
#include "var/var.hpp"
#include "var/bool.hpp"
#include "var/var_ops.hpp"

#include "robot/winutil/window_maze.hpp"
#include "robot/ideal_robot.hpp"

#include "winutil/engine/colors.hpp"
#include "winutil/screen.hpp"
#include "winutil/windows/window-output.hpp"
#include "winutil/windows/windows-row.hpp"
// clang-format on

#define COLOR_KEYWORDS COLOR_RGB(160, 149, 16)
// #define ASD COLOR_RGB(100, 100, 100)
// #define ASD COLOR_RGB(150, 120, 10)
#define OCEAN  COLOR_RGB(10, 160, 170)
#define BUTTER COLOR_RGB(245, 215, 156)
// GIRLIE PALLETTE
#define LIGHT_BLUE COLOR_RGB(179, 222, 226)
#define PINK_FROST COLOR_RGB(239, 207, 227)
#define ZEMLINIKA  COLOR_RGB(226, 115, 150)
#define PINK_MIST  COLOR_RGB(235, 154, 178)
#define BEIGE      COLOR_RGB(236, 242, 216)

// SUMMER PALLETTE
#define LEMONADE COLOR_RGB(242, 214, 161)
#define LIMONCH  COLOR_RGB(241, 168, 5)

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void print_env(Winutil::WindowOutput &w, const var::var_type &env) {
    auto &dim = var::get_dim(env);
    w.write(L"Walls:\n");
    for (unsigned i = 0; i < dim[0]; ++i) {
        w.write(L"[ ");
        for (unsigned j = 0; j < dim[1]; ++j) {
            auto val =
                std::get<var::Var<bool_t>>(var::idx(env, {i + 1, j + 1}));
            if (i == dim[0] / 2 && j == dim[1] / 2) w.write(L"()");
            else w.write(val[0] ? L"##" : L"  ");
        }
        w.write(L" ]\n");
    }
    w.write(L"Exits:\n");
    for (unsigned i = 0; i < dim[0]; ++i) {
        w.write(L"[ ");
        for (unsigned j = 0; j < dim[1]; ++j) {
            auto val =
                std::get<var::Var<bool_t>>(var::idx(env, {i + 1, j + 1}));
            if (i == dim[0] / 2 && j == dim[1] / 2) w.write(L"()");
            else w.write(val[1] ? L"##" : L"  ");
        }
        w.write(L" ]\n");
    }
}

int main(void) {
    std::setlocale(LC_ALL, "en_US.utf8");

    Winutil::Screen screen(
        Winutil::Screen::max_width(),
        Winutil::Screen::max_height() - 30
    );

    std::signal(SIGINT, Winutil::Screen::destroy_handler);

    auto &main_row_w = screen.make_window<Winutil::WindowsRow>();
    auto &maze_w = main_row_w.make_window<robot::WindowMaze>();
    auto &debug_w = main_row_w.make_window<Winutil::WindowOutput>();

    using namespace Winutil::engine;
    using namespace robot;
    using direction = Maze::direction;
    using position = Maze::position;

    maze_w.open("asd.maze");

    auto &maze = maze_w.get_maze();
    IdealRobot rb(maze);

    maze_w.draw_maze();
    print_env(debug_w, rb.get_env());

    screen.update();

#define MV  move = true;
#define UPD update = true;

    direction dir;
    while (kbhit()) {
        bool move = false;
        bool update = false;

        // clang-format off
        switch (getchar()) {
        case 'h': dir = direction::LEFT;     MV  break;
        case 'j': dir = direction::DOWN;     MV  break;
        case 'k': dir = direction::UP;       MV  break;
        case 'l': dir = direction::RIGHT;    MV  break;

        case 'w': maze.move_robot();         UPD break;
        case 'a': maze.rotate_robot_left();  UPD break;
        case 'd': maze.rotate_robot_right(); UPD break;
        case EOF: goto end;
        }
        // clang-format on

        if (move) {
            update = true;
            maze.set_robot_direction(dir);
            maze.move_robot();
        }
        if (update) {
            debug_w.clear();
            print_env(debug_w, rb.get_env());
            screen.update();
        }
    }
end:
}
