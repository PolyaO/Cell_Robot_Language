#pragma once
#include <stdexcept>

class InterpreterRuntimeError : public std::runtime_error {
   public:
    explicit InterpreterRuntimeError(const std::string &msg) noexcept
        : std::runtime_error(msg) {}
};

