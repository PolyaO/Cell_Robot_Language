#include "robot/ideal_robot.hpp"
#include "robot/paranoid.hpp"
#include <debugger/debugger.hpp>
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
    std::setlocale(LC_ALL, "en_US.utf8");

    if (argc != 3) {
        std::wcout << L"Usage: ./parse <maze_filename> <program_filename>"
                   << std::endl;
        return 1;
    }

    unsigned screen_width = Winutil::Screen::max_width();
    unsigned screen_height = Winutil::Screen::max_height() - 2;
    Winutil::Screen screen(screen_width, screen_height);

    robot::ParanoiaConfigGenerator gen(123);

    debug::Debugger dbg(screen, [&gen](robot::Maze &maze) {
        // return std::make_unique<robot::IdealRobot>(maze);
        return std::make_unique<robot::Paranoid>(maze, gen);
    });

    dbg.load(argv[1], argv[2]);

    dbg.start_interactive_execution();
}
