#pragma once

#include "debugger/debugger.hpp"

namespace debug::handlers {

#define CLEAR_CMD      L"clear"
#define CONTINUE_CMD   L"continue"
#define BREAKPOINT_CMD L"breakpoint"
#define HELP_CMD       L"help"
#define NEXT_CMD       L"next"
#define PRINT_CMD      L"print"
#define QUIT_CMD       L"quit"

typedef Debugger::op_result (*handler_t)(Debugger &, std::wstring_view);

Debugger::op_result clear_handler(Debugger &, std::wstring_view);
Debugger::op_result continue_handler(Debugger &, std::wstring_view);
Debugger::op_result breakpoint_handler(Debugger &, std::wstring_view);
Debugger::op_result help_handler(Debugger &, std::wstring_view);
Debugger::op_result next_handler(Debugger &, std::wstring_view);
Debugger::op_result print_handler(Debugger &, std::wstring_view);
Debugger::op_result quit_handler(Debugger &, std::wstring_view);

extern std::vector<std::tuple<std::wstring, handler_t, std::wstring>> handlers;

} // namespace debug::handlers
