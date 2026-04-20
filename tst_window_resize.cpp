#include <algorithm>
#include <array>
#include <clocale>
#include <csignal>
#include <cstdio>

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#undef COLOR
#undef CTRL

// clang-format off
#include "winutil/windows/window-file-view.hpp"
#include "winutil/windows/windows-row.hpp"
#include "winutil/engine/colors.hpp"
#include "winutil/screen.hpp"
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

#define HORIZONTAL_NUM 3

int main(void) {
    std::setlocale(LC_ALL, "en_US.utf8");

    Winutil::Screen screen(
        Winutil::Screen::max_width(),
        Winutil::Screen::max_height() - 30
    );

    std::signal(SIGINT, Winutil::Screen::destroy_handler);

    auto &main_row_w = screen.make_window<Winutil::WindowsRow>();

    using namespace Winutil::engine;
    using namespace Winutil;

    SyntaxHighlighter standard_highlight(
        {
            {Pattern::word(
                 L"struct|char_type|char|wchar_t|void|uint32_t|"
                 L"int|size_t|bool|auto"
             ),
             COLOR(LIMONCH, BLACK)},
            {Pattern::word(L"const_cast|const|for|if|else|while|do|static"),
             COLOR(ZEMLINIKA, BLACK)},
            {Pattern::word(L"noexcept|this|operator|using"),
             COLOR(BEIGE, BLACK)},
            {Pattern::wild(L"#include|#pragma|#define"),
             COLOR(PINK_FROST, BLACK)},
            {Pattern::word(L"namespace|static_assert"), COLOR(BEIGE, BLACK)},
        },
        COLOR(LIGHT_BLUE, BLACK)
    );

    std::array<Winutil::WindowFileView *, HORIZONTAL_NUM> file_w;
    std::array<unsigned, HORIZONTAL_NUM> horiz_sizes;

    for (int i = 0; i < HORIZONTAL_NUM; ++i) {
        auto &w = main_row_w.make_window<WindowFileView>();
        w.set_highlighter(standard_highlight);
        w.open("../winutil/src/winutil/windows/window-file-view.cpp");
        file_w[i] = &w;
    }

    for (int i = 0; i < HORIZONTAL_NUM; ++i) {
        horiz_sizes[i] = file_w[i]->get_size().width;
    }

    unsigned horiz_no = 0;
    unsigned line = 5;

    file_w[horiz_no]->select({line, 1}, {line, (unsigned)-1});

    screen.update();

#define RES resize = true;
#define SEL select = true;
    while (kbhit()) {
        bool resize = false;
        bool select = false;

        // clang-format off
        switch (getchar()) {
        case 'H':
            horiz_no = std::max(0, (int)horiz_no-1);
                                                      ;     ; SEL ;
            break;
        case 'L':
            horiz_no = std::min((unsigned)horiz_sizes.size()-1, horiz_no+1);
                                                      ;     ; SEL ;
            break;

        case 'h':
            horiz_sizes[horiz_no] = std::max(3u, horiz_sizes[horiz_no]-1);
                                                      ; RES ;     ;
            break;
        case 'j':
            line = std::min((unsigned)file_w[horiz_no]->last_line_no(), line+1);
                                                      ;     ; SEL ;
            break;
        case 'k':
            line = std::max(1u, line - 1);
                                                      ;     ; SEL ;
            break;
        case 'l':
            horiz_sizes[horiz_no] += 1;
                                                      ; RES ;     ;
            break;
        case 't':
            horiz_sizes[horiz_no] = 3;
                                                      ; RES ;     ;
            break;
        case 'q':
            goto end;
        }
        // clang-format on

        if (resize) { main_row_w.resize_windows(horiz_sizes); }

        if (select) {
            file_w[horiz_no]->select({line, 1}, {line, (unsigned)-1});
            file_w[horiz_no]->scroll_to(line, true);
        }

        screen.update();

        if (select) { file_w[horiz_no]->clear_selection(); }
    }
end:
}

