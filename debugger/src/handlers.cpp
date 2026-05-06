#include "debugger/debugger.hpp"
#include <debugger/handlers.hpp>

namespace debug::handlers {

// clang-format off
std::vector<std::tuple<std::wstring, handler_t, std::wstring>> handlers = {
    {CLEAR_CMD,      &clear_handler,      L"clear output window"         },
    {CONTINUE_CMD,   &continue_handler,   L"continue till the next break"},
    {BREAKPOINT_CMD, &breakpoint_handler, L"set breakpoint at line"      },
    {HELP_CMD,       &help_handler,       L"print this help"             },
    {NEXT_CMD,       &next_handler,       L"do the step"                 },
    {PRINT_CMD,      &print_handler,      L"print variable by name"      },
    {QUIT_CMD,       &quit_handler,       L"quit debugger"               },
};
// clang-format on

Debugger::op_result continue_handler(Debugger &d, std::wstring_view) {
    d.step_continue(3000);
    if (d.is_end_of_execution()) return Debugger::EXIT_FOUND;
    return Debugger::OK;
}

Debugger::op_result breakpoint_handler(Debugger &d, std::wstring_view arg) {
    auto first_arg = *(arg | std::views::split(L' ')).begin();
    std::wstring line_arg(first_arg.begin(), first_arg.end());
    int line_no;
    try {
        line_no = std::stoi(line_arg);
    } catch (const std::exception &e) {
        d.get_debug_w() << L"Invalid numeric: " << arg << L"\n";
        return Debugger::INVALID_FORMAT;
    }

    if (line_no <= 0) {
        d.get_debug_w() << L"Line number must be positive!\n";
        return Debugger::INVALID_FORMAT;
    }

    d.set_breakpoint(line_no);

    return Debugger::OK;
}

Debugger::op_result quit_handler(Debugger &, std::wstring_view) {
    return Debugger::QUIT;
}

Debugger::op_result help_handler(Debugger &d, std::wstring_view) {
    d.get_debug_w() << L"help:\n";
    for (auto &&[cmd, handler, desc] : handlers) {
        d.get_debug_w() << L"  " << cmd << L"\t" << desc << L"\n";
    };
    return Debugger::OK;
}

Debugger::op_result clear_handler(Debugger &d, std::wstring_view) {
    d.clear_output();
    return Debugger::OK;
}

Debugger::op_result print_handler(Debugger &d, std::wstring_view arg) {
    auto w_var_name = arg.substr(0, arg.find(L' '));
    auto var_name =
        w_var_name
        | std::views::transform([](wchar_t c) -> char { return (char)c; })
        | std::ranges::to<std::string>();
    auto var = d.get_var(var_name);
    if (!var) {
        d.get_debug_w() << L"no such vaiable: `"
                    << std::wstring(var_name.begin(), var_name.end())
                    << (L"`\n");
    } else d.print_variable(var.value(), w_var_name);
    return Debugger::OK;
}

Debugger::op_result next_handler(Debugger &d, std::wstring_view arg) {
    d.step_into();
    if (d.is_end_of_execution()) return Debugger::EXIT_FOUND;
    return Debugger::OK;
}

};
