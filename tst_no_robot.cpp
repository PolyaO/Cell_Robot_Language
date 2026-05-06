#include <algorithm>
#include <clocale>
#include <csignal>
#include <cstdlib>
#include <cwchar>
#include <iostream>
#include <memory>
#include <ostream>
#include <ranges>
#include <string>
#include <string_view>
#include <unistd.h>
#include <variant>
#include <vector>

#include "interpreter/exec/driver.hpp"
#include "robot/maze.hpp"
#include "var/bool.hpp"
#include "var/var.hpp"
#include "var/var_ops.hpp"

#include "robot/ideal_robot.hpp"
#include "robot/windows/window_maze.hpp"

#include "winutil/engine/syntax-highlighter.hpp"
#include "winutil/screen.hpp"
#include "winutil/windows/window-file-view.hpp"
#include "winutil/windows/window-output.hpp"
#include "winutil/windows/windows-column.hpp"
#include "winutil/windows/windows-row.hpp"

// GIRLIE PALLETTE
#define LIGHT_BLUE COLOR_RGB(179, 222, 226)
#define PINK_FROST COLOR_RGB(239, 207, 227)
#define ZEMLINIKA  COLOR_RGB(226, 115, 150)
#define PINK_MIST  COLOR_RGB(235, 154, 178)
#define BEIGE      COLOR_RGB(236, 242, 216)

// SUMMER PALLETTE
#define LEMONADE COLOR_RGB(242, 214, 161)
#define LIMONCH  COLOR_RGB(241, 168, 5)

namespace std {
wstring to_wstring(bool_t b) { return b ? L"T" : L"F"; }
} // namespace std

template <class T>
void print_variable_value(const var::Var<T> &var, Winutil::WindowOutput &w) {
    const var::dim_t &dim = var.get_dim();
    if (dim.size() == 1 && dim[0] == 1) {
        w << std::to_wstring(var[0]);
        return;
    }
    w << L"{";
    for (unsigned i = 1; i < dim[0]; ++i) {
        print_variable_value(var._idx({i}), w);
        w << L",";
    }
    print_variable_value(var._idx({dim[0]}), w);
    w << L"}";
}

void print_variable_value(const var::var_type &var, Winutil::WindowOutput &w) {
    if (std::holds_alternative<var::Var<int>>(var)) {
        print_variable_value(std::get<var::Var<int>>(var), w);
    } else {
        print_variable_value(std::get<var::Var<bool_t>>(var), w);
    }
}

void print_variable(
    const var::var_type &var, std::wstring_view name, Winutil::WindowOutput &w
) {
    if (std::holds_alternative<var::Var<int>>(var)) {
        w << L"IntVar: ";
    } else {
        w << L"BoolVar: ";
    }

    const var::dim_t &dim = var::get_dim(var);

    w << name;
    w << L" [";
    for (int i = 0; i < dim.size() - 1; ++i) {
        w << std::to_wstring(dim[i]);
        w << L", ";
    }
    w << std::to_wstring(dim.back());
    if (dim.size() < 3) {
        w << L"] = {";
        print_variable_value(var, w);
        w << L"}";
    } else {
        w << L"] = {\n";
        print_variable_value(var, w);
        w << L"\n}";
    }
    w << L"\n";
}

void print_env(Winutil::WindowOutput &w, const var::var_type &env) {
    auto &dim = var::get_dim(env);
    w << L"Walls:\n";
    for (unsigned i = 0; i < dim[0]; ++i) {
        w << L"[ ";
        for (unsigned j = 0; j < dim[1]; ++j) {
            auto val =
                std::get<var::Var<bool_t>>(var::idx(env, {i + 1, j + 1}));
            if (i == dim[0] / 2 && j == dim[1] / 2) w << L"()";
            else w << (val[0] ? L"##" : L"  ");
        }
        w << L" ]\n";
    }
    w << L"Exits:\n";
    for (unsigned i = 0; i < dim[0]; ++i) {
        w << L"[ ";
        for (unsigned j = 0; j < dim[1]; ++j) {
            auto val =
                std::get<var::Var<bool_t>>(var::idx(env, {i + 1, j + 1}));
            if (i == dim[0] / 2 && j == dim[1] / 2) w << L"()";
            else w << (val[1] ? L"##" : L"  ");
        }
        w << L" ]\n";
    }
}

#define CLEAR_CMD      L"clear"
#define CONTINUE_CMD   L"continue"
#define BREAKPOINT_CMD L"breakpoint"
#define HELP_CMD       L"help"
#define NEXT_CMD       L"next"
#define PRINT_CMD      L"print"
#define QUIT_CMD       L"quit"

enum handle_result { OK, INVALID_FORMAT, INVALID_COMMAND, QUIT, EXIT_FOUND };
struct handler_context {
    Winutil::WindowOutput &debug_w;
    Winutil::WindowFileView &code_w;
    Winutil::Screen &screen;
    exec::Driver &drv;
    robot::Maze &maze;
    std::vector<unsigned> breakpoints{};
};
typedef handle_result (*handler_t)(std::wstring_view, handler_context &);

handle_result clear_handler(std::wstring_view, handler_context &);
handle_result continue_handler(std::wstring_view, handler_context &);
handle_result breakpoint_handler(std::wstring_view, handler_context &);
handle_result help_handler(std::wstring_view, handler_context &);
handle_result next_handler(std::wstring_view, handler_context &);
handle_result print_handler(std::wstring_view, handler_context &);
handle_result quit_handler(std::wstring_view, handler_context &);

static std::vector<std::tuple<std::wstring, handler_t, std::wstring>> handlers =
    {
        {CLEAR_CMD, &clear_handler, L"clear output window"},
        {CONTINUE_CMD, &continue_handler, L"continue till the next break"},
        {BREAKPOINT_CMD, &breakpoint_handler, L"set breakpoint at line"},
        {HELP_CMD, &help_handler, L"print this help"},
        {NEXT_CMD, &next_handler, L"do the step"},
        {PRINT_CMD, &print_handler, L"print variable by name"},
        {QUIT_CMD, &quit_handler, L"quit debugger"},
};

handle_result continue_handler(std::wstring_view, handler_context &ctx) {
    unsigned next_line = 0;
    auto robot_pos = ctx.maze.get_robot_position();
    auto robot_dir = ctx.maze.get_robot_direction();
    while (true) {
        ctx.drv.exec_next();
        next_line = ctx.drv.get_next_lineno();
        if (next_line == 0) return EXIT_FOUND;
        if (std::ranges::find(ctx.breakpoints, next_line)
            != ctx.breakpoints.end())
            break;
        if (robot_pos != ctx.maze.get_robot_position()
            || robot_dir != ctx.maze.get_robot_direction()) {
            ctx.screen.update();
            usleep(3000);
        }
    }
    ctx.code_w.select({next_line, 0}, {next_line, (unsigned)-1});
    ctx.code_w.scroll_to(next_line, true);
    return OK;
}

handle_result breakpoint_handler(std::wstring_view arg, handler_context &ctx) {
    auto first_arg = *(arg | std::views::split(L' ')).begin();
    std::wstring line_arg(first_arg.begin(), first_arg.end());
    int line_no;
    try {
        line_no = std::stoi(line_arg);
    } catch (const std::exception &e) {
        ctx.debug_w << L"Invalid numeric: " << arg << L"\n";
        return INVALID_FORMAT;
    }

    if (line_no <= 0) {
        ctx.debug_w << L"Line number must be positive!\n";
        return INVALID_FORMAT;
    }

    ctx.breakpoints.push_back(line_no);

    return OK;
}

handle_result quit_handler(std::wstring_view, handler_context &) {
    return QUIT;
}

handle_result help_handler(std::wstring_view, handler_context &ctx) {
    ctx.debug_w << L"help:\n";
    for (auto &&[cmd, handler, desc] : handlers) {
        ctx.debug_w << L"  " << cmd << L"\t" << desc << L"\n";
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
        ctx.debug_w << L"no such vaiable: `"
                    << std::wstring(var_name.begin(), var_name.end())
                    << (L"`\n");
    } else print_variable(var.value(), w_var_name, ctx.debug_w);
    return OK;
}

handle_result next_handler(std::wstring_view arg, handler_context &ctx) {
    ctx.drv.exec_next();
    unsigned next_line = ctx.drv.get_next_lineno();
    if (next_line == 0) return EXIT_FOUND;
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
            else return INVALID_COMMAND;
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

    unsigned screen_width = Winutil::Screen::max_width();
    unsigned screen_height = Winutil::Screen::max_height() - 2;
    Winutil::Screen screen(screen_width, screen_height);
    std::signal(SIGINT, Winutil::Screen::destroy_handler);

    auto &main_row_w = screen.make_window<Winutil::WindowsRow>();
    auto &file_w = main_row_w.make_window<Winutil::WindowFileView>();
    auto &right_side_w = main_row_w.make_window<Winutil::WindowsColumn>();
    auto &upper_row_w = right_side_w.make_window<Winutil::WindowsRow>();
    auto &maze_w = upper_row_w.make_window<robot::WindowMaze>();
    auto &walls_exits_w = upper_row_w.make_window<Winutil::WindowOutput>();
    auto &debug_w = right_side_w.make_window<Winutil::WindowOutput>();

    maze_w.open(argv[1]);
    exec::Driver drv;
    drv.initialize(
        argv[2], std::make_unique<robot::IdealRobot>(maze_w.get_maze())
    );

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
                 L"MOVE|ROTATE_LEFT|ROTATE_RIGHT|GET_ENVIRONMENT|"
                 L"PLEASE|THANKS"
             ),
             COLOR(PINK_FROST, BLACK)},
        },
        COLOR(BEIGE, BLACK)
    );

    maze_w.draw_maze();
    print_env(walls_exits_w, drv.get_env().value());

    file_w.set_highlighter(standard_highlight);
    file_w.open(argv[2]);

    screen.update();

    std::wstring prompt = L"\r\e[2K> ";

    handler_context ctx = {
        .debug_w = debug_w,
        .code_w = file_w,
        .screen = screen,
        .drv = drv,
        .maze = maze_w.get_maze(),
    };

    while (true) {
        std::wstring line = input_line(prompt);
        switch (handle_input(line, ctx)) {
        case INVALID_COMMAND:
            debug_w << L"unknown command `" << line
                    << L"`, enter `h` or `help` for help\n";
            break;
        case EXIT_FOUND: debug_w << L"Exit was found!\n"; break;
        case QUIT: return 0;
        case INVALID_FORMAT: break;
        case OK: break;
        }
        walls_exits_w.clear();
        print_env(walls_exits_w, drv.get_env().value());
        screen.update();
    }
}
