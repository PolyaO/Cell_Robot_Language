
#pragma once
#include <format>
#include <stdexcept>
#include <string_view>

class InterpreterBuildError : public std::runtime_error {
   public:
    explicit InterpreterBuildError(const std::string &msg) noexcept
        : std::runtime_error(msg) {}
};

class ArgNameRepeat : public InterpreterBuildError {
   public:
    ArgNameRepeat(std::string_view name, unsigned line)
        : InterpreterBuildError(
              std::format("[{}] Argument name \"{}\" repeat.", line, name)) {}
};

class ArgNameUnknown : public InterpreterBuildError {
   public:
    ArgNameUnknown(std::string_view name, unsigned line)
        : InterpreterBuildError(
              std::format("[{}] Argument name \"{}\" unknown.", line, name)) {}
};

class ResNameUnknown : public InterpreterBuildError {
   public:
    ResNameUnknown(std::string_view name, std::string_view task, unsigned line)
        : InterpreterBuildError(
              std::format("[{}] In Task {} declaration. Var Name \"{}\" used "
                          "to store res is unknown",
                          line, task, name)) {}
};

class VarRedeclare : public InterpreterBuildError {
   public:
    VarRedeclare(std::string_view name, unsigned decl_line,
                 unsigned redecl_line)
        : InterpreterBuildError(
              std::format("[{}] Variable redeclare. Variable name: \"{}\". "
                          "first declared on line {}",
                          decl_line, name, redecl_line)) {}
};

class TaskRedeclare : public InterpreterBuildError {
   public:
    TaskRedeclare(std::string_view name, unsigned decl_line,
                  unsigned redecl_line)
        : InterpreterBuildError(std::format("[{}] Task redeclare. Task name: "
                                            "\"{}\". first declared on line {}",
                                            decl_line, name, redecl_line)) {}
};

class VarUnknown : public InterpreterBuildError {
   public:
    VarUnknown(std::string_view name, unsigned line)
        : InterpreterBuildError(
              std::format("[{}] Unknown Variable: \"{}\" used.", line, name)) {}
};

class TaskUnknown : public InterpreterBuildError {
   public:
    TaskUnknown(std::string_view name, unsigned line)
        : InterpreterBuildError(std::format(
              "[{}] Unknown Task name: \"{}\" used.", line, name)) {}
};

class DoubleLogicLiteral : public InterpreterBuildError {
   public:
    DoubleLogicLiteral(std::string_view boolean, unsigned line)
        : InterpreterBuildError(std::format(
              "[{}] In SWITCH declaration double {}.", line, boolean)) {}
};

class WrongDim : public InterpreterBuildError {
   public:
    WrongDim(int dim, unsigned line)
        : InterpreterBuildError(std::format(
              "[{}] In dimension list wrong dimension: {} .", line, dim)) {}
};

class WrongDimIdx : public InterpreterBuildError {
   public:
    WrongDimIdx(int dim, unsigned line)
        : InterpreterBuildError(std::format(
              "[{}] Wrong dimension idx: {} .", line, dim)) {}
};

class WrongChange : public InterpreterBuildError {
   public:
    WrongChange(int c, unsigned line)
        : InterpreterBuildError(std::format(
              "[{}] Wrong change: {} . Change should be >= 0", line, c)) {}
};
