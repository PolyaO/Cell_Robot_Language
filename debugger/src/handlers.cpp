#include "debugger/debugger.hpp"
#include "var/var.hpp"
#include "var/var_ops.hpp"
#include <debugger/handlers.hpp>
#include <ranges>
#include <regex>
#include <string>

namespace debug::handlers {

#define CLEAR_CMD      L"clear"
#define CONTINUE_CMD   L"continue"
#define BREAKPOINT_CMD L"breakpoint"
#define HELP_CMD       L"help"
#define NEXT_CMD       L"next"
#define PRINT_CMD      L"print"
#define QUIT_CMD       L"quit"

#define PARENT(_match) L"(?:" _match L")"
#define WS             L"\\s*"
#define OR             L"|"
#define UNSIGNED       L"[1-9][0-9]*"
#define DECIMAL        L"-?" UNSIGNED
#define HEX            L"0x[0-9a-fA-F]+"
#define ID             L"[a-zA-Z_][a-zA-Z0-9_]*"
#define EMPTY          L""
#define INDEX          L"\\[" WS PARENT(UNSIGNED L"," WS) L"*" UNSIGNED WS L"\\]"

#define MATCH(_match) L"(" _match L")"
#define MATCH_ANY     WS MATCH(ANY)

#define COMMAND_HANDLER(_cmd) \
    Debugger::op_result _cmd##_handler(Debugger &d, std::wstring_view args_line)

#define REGISTER_COMMAND(_cmd, _desc) {L## #_cmd, _cmd##_handler, L##_desc}

#define ARGPARSE(_content)                              \
    static std::wregex re(                              \
        WS PARENT(_content) WS,                         \
        std::wregex::ECMAScript | std::wregex::optimize \
    );                                                  \
    std::wstring __args_line_str(args_line);            \
    std::wsmatch args;                                  \
    if (!std::regex_match(__args_line_str, args, re))   \
        return Debugger::INVALID_FORMAT;
#define CHECK_EXECUTION                                     \
    if (d.is_end_of_execution()) {                          \
        d.get_debug_w() << L"Programm is ended already!\n"; \
        return Debugger::OK;                                \
    }

var::dim_t parse_idx(const std::wstring &idx_str) {
    static std::wregex re(
        UNSIGNED, std::wregex::ECMAScript | std::wregex::optimize
    );

    std::wsregex_iterator it(idx_str.begin(), idx_str.end(), re);
    std::wsregex_iterator end;

    var::dim_t res;
    for (; it != end; ++it) { res.push_back(std::stoi(it->str())); }

    return res;
}

COMMAND_HANDLER(continue) {
    CHECK_EXECUTION
    ARGPARSE(MATCH(EMPTY));
    d.step_continue(3000);
    if (d.is_end_of_execution()) return Debugger::EXIT_FOUND;
    return Debugger::OK;
}

COMMAND_HANDLER(break) {
    CHECK_EXECUTION
    ARGPARSE(MATCH(UNSIGNED) OR MATCH(EMPTY));

    unsigned line_no = 0;
    if (!args[1].str().empty()) {
        line_no = std::stoi(args[1]);
    } else {
        line_no = d.get_exec_line();
    }

    d.set_breakpoint(line_no);

    return Debugger::OK;
}

COMMAND_HANDLER(quit) {
    ARGPARSE(MATCH(EMPTY));
    return Debugger::QUIT;
}

COMMAND_HANDLER(help) {
    ARGPARSE(MATCH(EMPTY));

    d.get_debug_w() << L"help:\n";
    for (auto &&[cmd, handler, desc] : handlers) {
        d.get_debug_w() << L"  " << cmd << L"\t" << desc << L"\n";
    };
    return Debugger::OK;
}

COMMAND_HANDLER(clear) {
    ARGPARSE(MATCH(EMPTY));

    d.clear_output();
    return Debugger::OK;
}

COMMAND_HANDLER(print) {
    ARGPARSE(MATCH(ID) WS PARENT(MATCH(INDEX) OR MATCH(EMPTY)));

    auto w_var_name = args[1].str();
    auto var_name =
        w_var_name
        | std::views::transform([](wchar_t c) -> char { return (char)c; })
        | std::ranges::to<std::string>();
    auto opt_variable = d.get_var(var_name);
    if (!opt_variable) {
        d.get_debug_w() << L"no such vaiable: `"
                        << std::wstring(var_name.begin(), var_name.end())
                        << (L"`\n");
        return Debugger::INVALID_FORMAT;
    }

    var::var_type variable = opt_variable.value();
    if (!args[2].str().empty()) {
        var::dim_t idx = parse_idx(args[2].str());
        variable = var::idx(opt_variable.value(), idx);
    }

    d.print_variable(variable, w_var_name);
    return Debugger::OK;
}

COMMAND_HANDLER(next) {
    CHECK_EXECUTION
    ARGPARSE(MATCH(UNSIGNED) OR MATCH(EMPTY));

    unsigned lines = 1;
    if (!args[1].str().empty()) lines = std::stoi(args[1]);

    while (lines-- != 0) {
        d.step_into();
        if (d.is_end_of_execution()) return Debugger::EXIT_FOUND;
    }
    return Debugger::OK;
}

// clang-format off
std::vector<std::tuple<std::wstring, handler_t, std::wstring>> handlers = {
    REGISTER_COMMAND(clear,    "clear output window"         ),
    REGISTER_COMMAND(continue, "continue till the next break"),
    REGISTER_COMMAND(break,    "set breakpoint at line"      ),
    REGISTER_COMMAND(help,     "print this help"             ),
    REGISTER_COMMAND(next,     "do the step"                 ),
    REGISTER_COMMAND(print,    "print variable by name"      ),
    REGISTER_COMMAND(quit,     "quit debugger"               ),
};
// clang-format on

}; // namespace debug::handlers
