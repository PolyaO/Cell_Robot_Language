#include <clocale>
#include <initializer_list>
#include <iostream>

#include "robot/winutil/window_maze.hpp"
#include "winutil/engine/syntax-highlighter.hpp"
#include "winutil/main-window.hpp"
#include "winutil/window-file-view.hpp"
#include "winutil/windows-row.hpp"

int main(void) {
    std::setlocale(LC_ALL, "en_US.utf8");

    Winutil::MainWindow main_w(std::wcout, Winutil::MainWindow::max_width(),
                               Winutil::MainWindow::max_height());

    auto &main_row_w = main_w.make_window<Winutil::WindowsRow>();
    auto &maze_w = main_row_w.make_window<robot::WindowMaze>();
    auto &w1 = main_row_w.make_window<Winutil::WindowFileView>();

    using namespace Winutil::engine;

    Winutil::engine::SyntaxHighlighter standard_highlight(
        {
            {Pattern::word(L"struct|char_type|char|wchar_t|void|uint32_t|"
                           L"int|size_t|bool|auto"),
             COLOR(GREEN, BLACK)},
            {Pattern::word(L"const_cast|const|for|if|else|while|do|static"),
             COLOR(RED, BLACK)},
            {Pattern::word(L"noexcept|this|operator|using"),
             COLOR(YELLOW, BLACK)},
            {Pattern::wild(L"#include|#pragma|#define"), COLOR(BLUE, BLACK)},
            {Pattern::word(L"namespace|static_assert"), COLOR(BLUE, BLACK)},
        },
        COLOR_NONE);

    // Я тут просто так затесался, чтобы namespace написать

    w1.set_highlighter(standard_highlight);
    // w2.set_highlighter(standard_highlight);
    // w3.set_highlighter(standard_highlight);

    w1.open("../winutil_test.cpp");
    maze_w.open("../asd.maze");
    // w2.open("../src/main.cpp");
    // w3.open("../include/winutil/engine/color-string.hpp");
    // w3.open("../src/winutil/window-file-view.cpp");

    // w3.select({14, 6}, {18, 47});

    maze_w.draw_maze();

    main_w.update();
}
