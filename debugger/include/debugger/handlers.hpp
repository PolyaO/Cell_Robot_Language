#pragma once

#include "debugger/debugger.hpp"

namespace debug::handlers {

typedef Debugger::op_result (*handler_t)(Debugger &, std::wstring_view);

extern std::vector<std::tuple<std::wstring, handler_t, std::wstring>> handlers;

} // namespace debug::handlers
