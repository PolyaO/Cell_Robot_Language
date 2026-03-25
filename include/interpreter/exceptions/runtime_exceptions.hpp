#pragma once
#include <stdexcept>

class InterpreterExecError : public std::runtime_error {
   public:
    explicit InterpreterExecError(const std::string &msg) noexcept
        : std::runtime_error(msg) {}
};

