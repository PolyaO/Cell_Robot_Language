#pragma once
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "expr.hpp"
namespace ast {
using variables = std::unordered_map<std::string, unsigned>;
struct AstCtx {
    std::vector<std::tuple<std::string, unsigned, variables>> tasks_meta;
    std::vector<expr> tasks;
    unsigned findexit_idx;
};
}  // namespace ast
