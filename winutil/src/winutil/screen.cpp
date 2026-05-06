#include "winutil/engine/common.hpp"
#include <asm-generic/ioctls.h>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#undef COLOR
#undef CTRL

// clang-format off
#include "winutil/window.hpp"
#include "winutil/screen.hpp"
#include "winutil/engine/color-string.hpp"
#include "winutil/engine/draw-area.hpp"
#include "winutil/engine/strdiff.hpp"
// clang-format on

#define CURSOR_AT_START          L"\e[1;1f"
#define CURSOR_SAVE              L"\e[s"
#define CURSOR_RESTORE           L"\e[u"
#define CURSOR_HIDE              L"\e[?25l"
#define CURSOR_UNHIDE            L"\e[?25h"
#define ALTERNATE_SCREEN_ENABLE  L"\e[?1049h"
#define ALTERNATE_SCREEN_DISABLE L"\e[?1049l"

static std::wostream &out = std::wcout;
static bool alternative_screen_enabled = false;

static inline void disable_echo() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static inline void enable_echo() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

namespace Winutil {

Screen::Screen(unsigned width, unsigned height) :
    main_area(width, height), repl_area(width, height) {}

Screen::~Screen() {
    if (alternative_screen_enabled) {
        out << ALTERNATE_SCREEN_DISABLE;
        out.flush();
        alternative_screen_enabled = false;
    }
}

void Screen::destroy_handler(int signal) noexcept {
    if (alternative_screen_enabled) {
        out << ALTERNATE_SCREEN_DISABLE;
        out.flush();
        alternative_screen_enabled = false;
    }
    std::exit(0);
}

unsigned Screen::max_width() noexcept {
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

unsigned Screen::max_height() noexcept {
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

void Screen::clear() { _main->clear(); }

void Screen::update() {
    _main->update();
    std::wstring_view color = COLOR_NONE;
    unsigned line = 1;
    auto move_cursor_rigth = [this](unsigned ncols) {
        if (ncols != 0) { out << L"\e[" << ncols << L'C'; }
    };
    auto output = [this, &color](engine::color_string_view str) {
        for (auto c : str) {
            if (c.get_color() != color) {
                color = c.get_color();
                out << color;
            }
            out << c.ch;
        }
    };
    auto newline = [this, &color, &line]() {
        out << L"\e[" << ++line << ";1H" << (color = COLOR_NONE);
    };

    using namespace std::views;

    if (!alternative_screen_enabled) {
        out << ALTERNATE_SCREEN_ENABLE;
        alternative_screen_enabled = true;
    }

    out << CURSOR_HIDE CURSOR_AT_START;
    for (auto &&[repl_line, curr_line] : zip(repl_area, main_area)) {
        for (auto &&[skip, diff] :
             strdiff_view<ColoredChar>(repl_line, curr_line, 6)) {
            move_cursor_rigth(skip);
            output(diff);
        }
        newline();
    }
    out << CURSOR_UNHIDE;
    out.flush();
    repl_area = main_area.copy();
}

std::wstring Screen::input(OutputWindow &w) {
    auto initial_cursor_pos = w.get_cursor();
    auto global_pos = w.get_area().get_info().global_pos + initial_cursor_pos;
    int str_idx = 0;

    auto place_cursor = [global_pos, &w](int str_pos) {
        out << L"\e[" << global_pos.row + 1 << L';'
            << global_pos.col + str_pos + 1 << L'H';
    };

    auto output =
        [global_pos, &w, &place_cursor, this](std::wstring &str, int str_pos) {
            place_cursor(0);
            out << str;
            update();
            place_cursor(str_pos);
        };

    std::wstring str;

    disable_echo();
    out << CURSOR_SAVE;
    update();
    place_cursor(0);

    wchar_t ch;

    while (std::wcin.get(ch) && ch != L'\n') {
        if (ch == L'\x1B') {
            std::wcin.get(ch);
            if (ch != L'[') continue;
            std::wcin.get(ch);
            switch (ch) {
            case L'A': break;
            case L'B': break;
            case L'C': str_idx = std::min((int)str.size(), str_idx + 1); break;
            case L'D': str_idx = std::max(0, str_idx - 1); break;
            }
            output(str, str_idx);
        } else {
            switch (ch) {
            case L'\t': break;
            case L'\b':
            case 127:
                str_idx -= 1;
                str.erase(str.begin() + str_idx);
                break;
            default:
                str.insert(str.begin() + str_idx, ch);
                str_idx += 1;
                break;
            }
            output(str, str_idx);
        }
    }

    enable_echo();
    out << CURSOR_RESTORE;

    w.write(str);
    w.write(L"\n");
    update();

    return str;
}

std::wstring
Screen::input(OutputWindow &w, std::vector<std::wstring> &history) {
    auto initial_cursor_pos = w.get_cursor();
    auto global_pos = w.get_area().get_info().global_pos + initial_cursor_pos;
    int str_idx = 0;

    auto place_cursor = [global_pos, &w](int str_pos) {
        out << L"\e[" << global_pos.row << L';' << global_pos.col + str_pos
            << L'H';
    };

    auto output =
        [global_pos, &w, &place_cursor, this](std::wstring &str, int str_pos) {
            place_cursor(0);
            out << str;
            place_cursor(str_pos);
        };

    std::wstring input_str;
    std::wstring *str = &input_str;

    disable_echo();
    out << CURSOR_SAVE;
    place_cursor(0);
    update();

    wchar_t ch;
    int history_idx = history.size();

    while (std::wcin.get(ch) && ch != L'\n') {
        if (ch == L'\x1B') {
            std::wcin.get(ch);
            if (ch != L'[') continue;
            std::wcin.get(ch);
            switch (ch) {
            case L'A':
                history_idx = std::max(0, history_idx - 1);
                str = &history[history_idx];
                str_idx = str->size();
                break;
            case L'B':
                history_idx = std::min((int)history.size(), history_idx + 1);
                if (history_idx == history.size()) str = &input_str;
                else str = &history[history_idx];
                str_idx = str->size();
                break;
            case L'C': str_idx = std::min((int)str->size(), str_idx + 1); break;
            case L'D': str_idx = std::max(0, str_idx - 1); break;
            }
        } else {
            if (history_idx != history.size()) {
                input_str = *str;
                str = &input_str;
            }
            if (ch == L'\t') continue;
            str->insert(str->begin() + str_idx, ch);
            str_idx += 1;
        }
        output(*str, str_idx);
    }

    enable_echo();
    out << CURSOR_RESTORE;

    w.write(*str);
    w.write(L"\n");
    update();

    return *str;
}

Window &Screen::get_window() { return *_main; }

void Screen::resize(unsigned width, unsigned height) {
    main_area.resize(width, height);
    _main->move(main_area.make_area());
}

}; // namespace Winutil
