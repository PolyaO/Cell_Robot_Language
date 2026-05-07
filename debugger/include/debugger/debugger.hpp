#pragma once

#include "interpreter/exec/driver.hpp"
#include "robot/maze.hpp"
#include "robot/robot.hpp"
#include "robot/windows/window_maze.hpp"
#include "var/var.hpp"
#include "winutil/screen.hpp"
#include "winutil/windows/window-file-view.hpp"
#include "winutil/windows/window-output.hpp"
#include "winutil/windows/windows-column.hpp"
#include "winutil/windows/windows-row.hpp"
#include <memory>
#include <optional>
#include <vector>

namespace debug {

class Debugger {
  public:
    using robot_getter_t =
        std::function<std::unique_ptr<robot::Robot>(robot::Maze &)>;

    enum op_result { OK, INVALID_FORMAT, INVALID_COMMAND, QUIT, EXIT_FOUND };

    Debugger(Winutil::Screen &screen, robot_getter_t robot_getter);

    void load();
    void load(std::string_view maze_filename, std::string_view prog_filename);

    void start_interactive_execution();
    void stop_programm() noexcept;

    bool is_end_of_execution() const noexcept { return prog_ended; }
    unsigned get_exec_line() const noexcept;

    op_result execute_cmd(std::wstring_view cmd);

    void step_into(int robot_move_delay = 0);
    void step_continue(int robot_move_delay = 0);

    void set_breakpoint(unsigned line) noexcept;
    void clear_breakpoint(unsigned line) noexcept;
    // void step_over();

    void print_env();
    void print_robot_stats();
    void print_variable(const var::var_type &var, std::wstring_view name);
    void clear_output() noexcept;

    void redraw_screen() noexcept;

    Winutil::WindowOutput &get_debug_w() noexcept { return *debug_w; }
    std::optional<var::var_type> get_var(std::string_view var_name) noexcept {
        return drv.get_var(var_name);
    }

  private:
    bool check_run_status() const noexcept;

    robot_getter_t robot_getter;

    Winutil::Screen &screen;

    Winutil::WindowsRow *main_row_w;
    Winutil::WindowsRow *upper_row_w;
    Winutil::WindowsColumn *right_side_w;

    robot::WindowMaze *maze_w;
    Winutil::WindowOutput *debug_w;
    Winutil::WindowOutput *env_w;
    Winutil::WindowFileView *file_w;

    exec::Driver drv;
    robot::Robot *robot;

    std::vector<unsigned> breaks;
    std::string maze_filename;
    std::string prog_filename;

    bool prog_ended = false;
};

} // namespace debug
