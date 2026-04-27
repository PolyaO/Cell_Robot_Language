#include <algorithm>
#include <clocale>
#include <csignal>
#include <cstdlib>
#include <cwchar>
#include <functional>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>
#include <string_view>
#include <variant>

#include "interpreter/exec/driver.hpp"
#include "var/var.hpp"

// cland-format off
#include <readline/readline.h>

#undef COLOR
#undef CTRL

#include "winutil/engine/syntax-highlighter.hpp"
#include "winutil/screen.hpp"
#include "winutil/windows/window-file-view.hpp"
#include "winutil/windows/window-output.hpp"
#include "winutil/windows/windows-column.hpp"
#include "winutil/windows/windows-row.hpp"

// cland-format on

// GIRLIE PALLETTE
#define LIGHT_BLUE COLOR_RGB(179, 222, 226)
#define PINK_FROST COLOR_RGB(239, 207, 227)
#define ZEMLINIKA  COLOR_RGB(226, 115, 150)
#define PINK_MIST  COLOR_RGB(235, 154, 178)
#define BEIGE      COLOR_RGB(236, 242, 216)

// SUMMER PALLETTE
#define LEMONADE COLOR_RGB(242, 214, 161)
#define LIMONCH  COLOR_RGB(241, 168, 5)

void print_variable(
    const var::var_type &a, std::wstring_view var, Winutil::WindowOutput &w
) {
    if (std::holds_alternative<var::Var<int>>(a)) {
        w.write(L"IntVar ");
    } else w.write(L"BoolVar ");
    std::visit(
        [&](auto &v) {
            auto val = v.get_val();
            auto dim = v.get_dim();
            w.write(var);
            w.write(L"\n");
            w.write(L"val: ");
            std::visit(
                [&](auto &mx) {
                    for (auto &e : mx) {
                        w.write(std::to_wstring(e));
                        w.write(L", ");
                    }
                    w.write(L"\n");
                },
                val
            );
            w.write(L"dim: ");
            for (auto &e : dim) {
                w.write(std::to_wstring(e));
                w.write(L", ");
            }
            w.write(L"\n");
        },
        a
    );
}

#define CLEAR_CMD L"clear"
#define HELP_CMD  L"help"
#define NEXT_CMD  L"next"
#define PRINT_CMD L"print"
#define QUIT_CMD  L"quit"

enum handle_result { OK, INVALID_FORMAT, INVALID_COMMAND, QUIT };
struct handler_context {
    Winutil::WindowOutput &debug_w;
    Winutil::WindowFileView &code_w;
    exec::Driver &drv;
};
typedef handle_result (*handler_t)(std::wstring_view, handler_context &);

handle_result clear_handler(std::wstring_view, handler_context &);
handle_result help_handler(std::wstring_view, handler_context &);
handle_result next_handler(std::wstring_view, handler_context &);
handle_result print_handler(std::wstring_view, handler_context &);
handle_result quit_handler(std::wstring_view, handler_context &);

static std::vector<std::tuple<std::wstring, handler_t, std::wstring>> handlers =
    {
        {CLEAR_CMD, &clear_handler, L"clear output window"},
        {HELP_CMD, &help_handler, L"print this help"},
        {NEXT_CMD, &next_handler, L"do the step"},
        {PRINT_CMD, &print_handler, L"print variable by name"},
        {QUIT_CMD, &quit_handler, L"quit debugger"},
};

handle_result quit_handler(std::wstring_view, handler_context &) {
    return QUIT;
}

handle_result help_handler(std::wstring_view, handler_context &ctx) {
    ctx.debug_w.write(L"help:\n");
    for (auto &&[cmd, handler, desc] : handlers) {
        ctx.debug_w.write(L"  ");
        ctx.debug_w.write(cmd);
        ctx.debug_w.write(L"\t");
        ctx.debug_w.write(desc);
        ctx.debug_w.write(L"\n");
    };
    return OK;
}

handle_result clear_handler(std::wstring_view, handler_context &ctx) {
    ctx.debug_w.clear();
    return OK;
}

handle_result print_handler(std::wstring_view arg, handler_context &ctx) {
    auto w_var_name = arg.substr(0, arg.find(L' '));
    auto var_name =
        w_var_name
        | std::views::transform([](wchar_t c) -> char { return (char)c; })
        | std::ranges::to<std::string>();
    auto var = ctx.drv.get_var(var_name);
    if (!var) {
        ctx.debug_w.write(L"no such vaiable: `");
        ctx.debug_w.write(std::wstring(var_name.begin(), var_name.end()));
        ctx.debug_w.write(L"`\n");
    } else print_variable(var.value(), w_var_name, ctx.debug_w);
    return OK;
}

handle_result next_handler(std::wstring_view arg, handler_context &ctx) {
    ctx.drv.exec_next();
    unsigned next_line = ctx.drv.get_next_lineno();
    ctx.code_w.select({next_line, 0}, {next_line, (unsigned)-1});
    ctx.code_w.scroll_to(next_line, true);
    return OK;
}

std::wstring input_line(std::wstring_view prompt) {
    char *line;
    size_t len;
    std::wstring res;
    std::wcout << prompt << std::flush;
    std::getline(std::wcin, res);
    return std::move(res);
}

handle_result handle_input(const std::wstring &input, handler_context &ctx) {
    std::vector<bool> matches(handlers.size(), true);

    auto space_pos = input.find(L' ');

    // determine which command is matched
    for (auto [chr_idx, chr] :
         std::views::enumerate(input.substr(0, space_pos))) {
        for (auto &&[hdl_idx, hdl_entry] : std::views::enumerate(handlers)) {
            auto &&[cmd, handler, desc] = hdl_entry;
            if (chr_idx >= cmd.length() || cmd[chr_idx] != chr)
                matches[hdl_idx] = false;
        }
    }

    // check that exactly one command is matched
    handler_t hdl = nullptr;
    for (auto &&[hdl_idx, hdl_entry] : std::views::enumerate(handlers)) {
        auto &&[cmd, handler, desc] = hdl_entry;
        if (matches[hdl_idx] == true) {
            if (hdl == nullptr) hdl = handler;
            else return INVALID_FORMAT;
        }
    }

    if (hdl == nullptr) return INVALID_COMMAND;
    while (space_pos < input.size() && input[space_pos] == L' ') space_pos += 1;
    if (space_pos == std::wstring::npos) return hdl(L"", ctx);
    return hdl(input.substr(space_pos), ctx);
}

int main(int argc, char *argv[]) {
    using namespace Winutil::engine;

    std::setlocale(LC_ALL, "en_US.utf8");

    if (argc != 3) {
        std::cout << "Usage: ./parse <maze_filename> <program_filename>"
                  << std::endl;
        return 1;
    }

    exec::Driver drv;
    unsigned lineno = drv.initialize(argv[1], argv[2]);
    unsigned screen_width = Winutil::Screen::max_width();
    unsigned screen_height = Winutil::Screen::max_height() - 2;

    Winutil::Screen screen(screen_width, screen_height);
    std::signal(SIGINT, Winutil::Screen::destroy_handler);

    auto &main_row_w = screen.make_window<Winutil::WindowsRow>();
    auto &debug_w = main_row_w.make_window<Winutil::WindowOutput>();
    auto &right_side_w = main_row_w.make_window<Winutil::WindowsColumn>();
    auto &robot_w = right_side_w.make_window<Winutil::WindowOutput>();
    auto &file_w = right_side_w.make_window<Winutil::WindowFileView>();

    SyntaxHighlighter standard_highlight(
        {
            {Pattern::word(
                 L"TASK|FINDEXIT|RESULT|DO|GET|FOR|BOUNDARY|STEP|SWITCH"
             ),
             COLOR(ZEMLINIKA, BLACK)},
            {Pattern::word(
                 L"NOT|MXTRUE|MXFALSE|MXEQ|MXLT|MXGT|MXLTE|MXGTE|ELEQ|ELLT|"
                 L"ELGT|ELLTE|ELGTE|GET|SIZE|REDUCE|EXTEND|OR|AND|FALSE|TRUE|["
                 L"0-9]+"
             ),
             COLOR(LIMONCH, BLACK)},
            {Pattern::wild(L"VAR|SIZE|LOGITIZE|DIGITIZE|REDUCE|EXTEND"),
             COLOR(LIGHT_BLUE, BLACK)},
            {Pattern::wild(
                 L"MOVE|ROTATE_LEFT|ROTATE_RIGHT|GET_ENVIRONMENT|PLEASE|THANKS"
             ),
             COLOR(PINK_FROST, BLACK)},
        },
        COLOR(BEIGE, BLACK)
    );

    file_w.set_highlighter(standard_highlight);
    file_w.open(argv[2]);

    robot_w.write(L"ROBOT HERE!\n");
    screen.update();

    std::wstring prompt = L"\r\e[2K> ";

    handler_context ctx = {.debug_w = debug_w, .code_w = file_w, .drv = drv};

    while (true) {
        std::wstring line = input_line(prompt);
        switch (handle_input(line, ctx)) {
        case INVALID_COMMAND:
            debug_w.write(L"unknown command `");
            debug_w.write(line);
            debug_w.write(L"`, enter `h` or `help` for help\n");
            break;
        case INVALID_FORMAT: debug_w.write(L"invalid arguments\n"); break;
        case QUIT: return 0;
        case OK: break;
        }
        screen.update();
    }
}
