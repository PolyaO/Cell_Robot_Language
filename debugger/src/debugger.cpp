#include "debugger/debugger.hpp"
#include "debugger/handlers.hpp"
#include "var/var.hpp"
#include "var/var_ops.hpp"
#include "winutil/engine/syntax-highlighter.hpp"
#include <iostream>
#include <string>

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

namespace debug {

using namespace Winutil::engine;

static Winutil::engine::SyntaxHighlighter standard_highlight(
    {
        {Pattern::word(L"TASK|FINDEXIT|RESULT|DO|GET|FOR|BOUNDARY|STEP|SWITCH"),
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

void print_variable_value(const var::var_type &var, Winutil::WindowOutput &w);

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

void Debugger::print_variable(
    const var::var_type &var, std::wstring_view name
) {
    auto &w = *debug_w;
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

void Debugger::print_env() {
    auto &w = *env_w;
    const var::var_type env = drv.get_env().value();
    auto &dim = var::get_dim(env);
    env_w->clear();
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

Debugger::op_result Debugger::execute_cmd(std::wstring_view input) {
    std::vector<bool> matches(handlers::handlers.size(), true);

    auto space_pos = input.find(L' ');

    // determine which command is matched
    for (auto [chr_idx, chr] :
         std::views::enumerate(input.substr(0, space_pos))) {
        for (auto &&[hdl_idx, hdl_entry] :
             std::views::enumerate(handlers::handlers)) {
            auto &&[cmd, handler, desc] = hdl_entry;
            if (chr_idx >= cmd.length() || cmd[chr_idx] != chr)
                matches[hdl_idx] = false;
        }
    }

    // check that exactly one command is matched
    handlers::handler_t hdl = nullptr;
    for (auto &&[hdl_idx, hdl_entry] :
         std::views::enumerate(handlers::handlers)) {
        auto &&[cmd, handler, desc] = hdl_entry;
        if (matches[hdl_idx] == true) {
            if (hdl == nullptr) hdl = handler;
            else return INVALID_COMMAND;
        }
    }

    if (hdl == nullptr) return INVALID_COMMAND;
    while (space_pos < input.size() && input[space_pos] == L' ') space_pos += 1;
    if (space_pos == std::wstring::npos) return hdl(*this, L"");
    return hdl(*this, input.substr(space_pos));
}

bool Debugger::check_run_status() const noexcept {
    if (prog_ended) {
        *debug_w << L"Programm's ended already\n";
        return false;
    }
    return true;
}

unsigned Debugger::get_exec_line() const noexcept {
    return drv.get_next_lineno();
}

void Debugger::step_into(int robot_move_delay) {
    check_run_status();
    auto &maze = maze_w->get_maze();
    auto robot_pos = maze.get_robot_position();
    auto robot_dir = maze.get_robot_direction();
    drv.exec_next();
    unsigned line = get_exec_line();
    if (line == 0) {
        stop_programm();
        return;
    }
    if (robot_pos != maze.get_robot_position()
        || robot_dir != maze.get_robot_direction()) {
        redraw_screen();
        usleep(robot_move_delay);
    }
    file_w->select({line, 0}, {line, (unsigned)-1});
    file_w->scroll_to(line, true);
}

void Debugger::step_continue(int robot_move_delay) {
    check_run_status();
    while (true) {
        step_into(robot_move_delay);
        if (is_end_of_execution()) return;
        if (std::ranges::find(breaks, get_exec_line()) != breaks.end()) break;
    }
    unsigned line = get_exec_line();
    file_w->select({line, 0}, {line, (unsigned)-1});
    file_w->scroll_to(line, true);
}

void Debugger::set_breakpoint(unsigned line) noexcept {
    auto pos = std::ranges::find(breaks, line);
    if (pos == breaks.end()) breaks.push_back(line);
}

void Debugger::clear_breakpoint(unsigned line) noexcept {
    auto pos = std::ranges::find(breaks, line);
    if (pos != breaks.end()) breaks.erase(pos);
}

void Debugger::clear_output() noexcept {
    debug_w->clear();
    redraw_screen();
}

Debugger::Debugger(Winutil::Screen &screen, robot_getter_t robot_getter) :
    screen(screen), robot_getter(robot_getter) {
    main_row_w = &screen.make_window<Winutil::WindowsRow>();
    file_w = &main_row_w->make_window<Winutil::WindowFileView>();
    right_side_w = &main_row_w->make_window<Winutil::WindowsColumn>();
    upper_row_w = &right_side_w->make_window<Winutil::WindowsRow>();
    maze_w = &upper_row_w->make_window<robot::WindowMaze>();
    env_w = &upper_row_w->make_window<Winutil::WindowOutput>();
    debug_w = &right_side_w->make_window<Winutil::WindowOutput>();
}

void Debugger::stop_programm() noexcept { prog_ended = true; }

void Debugger::load() {
    maze_w->open(maze_filename);
    drv.initialize(prog_filename, robot_getter(maze_w->get_maze()));
    file_w->set_highlighter(standard_highlight);
    file_w->open(prog_filename);
    maze_w->draw_maze();
    debug_w->clear();
    redraw_screen();
}

void Debugger::load(
    std::string_view maze_filename, std::string_view prog_filename
) {
    this->maze_filename = maze_filename;
    this->prog_filename = prog_filename;
    load();
}

void Debugger::redraw_screen() noexcept {
    print_env();
    screen.update();
}

std::wstring input_line(std::wstring_view prompt) {
    char *line;
    size_t len;
    std::wstring res;
    std::wcout << prompt << std::flush;
    std::getline(std::wcin, res);
    return std::move(res);
}

void Debugger::start_interactive_execution() {
    std::wstring prompt = L"\r\e[2K> ";

    redraw_screen();

    while (true) {
        std::wstring line = input_line(prompt);
        switch (execute_cmd(line)) {
        case INVALID_COMMAND:
            *debug_w << L"unknown command `" << line
                     << L"`, enter `h` or `help` for help\n";
            break;
        case EXIT_FOUND: *debug_w << L"Exit was found!\n"; break;
        case QUIT: return;
        case INVALID_FORMAT: break;
        case OK: break;
        }
        redraw_screen();
    }
}

} // namespace debug
