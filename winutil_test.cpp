#include <clocale>
#include <initializer_list>
#include <iostream>

#include "robot/winutil/window_maze.hpp"
#include "winutil/engine/syntax-highlighter.hpp"
#include "winutil/main-window.hpp"
#include "winutil/window-file-view.hpp"
#include "winutil/windows-row.hpp"

#define COLOR_KEYWORDS COLOR_RGB(160, 149, 16)
// #define ASD COLOR_RGB(100, 100, 100)
// #define ASD COLOR_RGB(150, 120, 10)
#define OCEAN COLOR_RGB(10, 160, 170)
#define BUTTER COLOR_RGB(245, 215, 156)
// GIRLIE PALLETTE
#define LIGHT_BLUE COLOR_RGB(179, 222, 226)
#define PINK_FROST COLOR_RGB(239, 207, 227)
#define ZEMLINIKA COLOR_RGB(226, 115, 150)
#define PINK_MIST COLOR_RGB(235, 154, 178)
#define BEIGE COLOR_RGB(236, 242, 216)

// SUMMER PALLETTE
#define LEMONADE COLOR_RGB(242, 214, 161)
#define LIMONCH COLOR_RGB(241, 168, 5)

int main(void) {
    std::setlocale(LC_ALL, "en_US.utf8");

    Winutil::MainWindow main_w(std::wcout, Winutil::MainWindow::max_width(),
                               Winutil::MainWindow::max_height());

    auto &main_row_w = main_w.make_window<Winutil::WindowsRow>();
    auto &w1 = main_row_w.make_window<Winutil::WindowFileView>();
    auto &maze_w = main_row_w.make_window<robot::WindowMaze>();
    // auto &w2 = main_row_w.make_window<Winutil::WindowFileView>();

    using namespace Winutil::engine;

    Winutil::engine::SyntaxHighlighter standard_highlight(
        {
            {Pattern::word(L"struct|char_type|char|wchar_t|void|uint32_t|"
                           L"int|size_t|bool|auto"),
             COLOR(LIMONCH, BLACK)},
            {Pattern::word(L"const_cast|const|for|if|else|while|do|static"),
             COLOR(ZEMLINIKA, BLACK)},
            {Pattern::word(L"noexcept|this|operator|using"),
             COLOR(BUTTER, BLACK)},
            {Pattern::wild(L"#include|#pragma|#define"),
             COLOR(PINK_FROST, BLACK)},
            {Pattern::word(L"namespace|static_assert"), COLOR(BEIGE, BLACK)},
        },
        COLOR(LIGHT_BLUE, BLACK));

    // Я тут просто так затесался, чтобы namespace написать

    w1.set_highlighter(standard_highlight);
    //w2.set_highlighter(standard_highlight);
    // w3.set_highlighter(standard_highlight);

    w1.open("../winutil_test.cpp");
    //w2.open("../main.cpp");
    maze_w.open("../asd.maze");
    // w2.open("../src/main.cpp");
    // w3.open("../include/winutil/engine/color-string.hpp");
    // w3.open("../src/winutil/window-file-view.cpp");

    w1.select({14, 6}, {18, 47});

    maze_w.draw_maze();

    main_w.update();
}
