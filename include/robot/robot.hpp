#pragma once
#include "backend/rvals/var.hpp"
#include <string_view>

namespace robot {
class Robot {
    public:
        Robot();
        void initialize(std::string_view filename);
        ast::Var get_env(bool is_politely_asked = false);
        void move(bool is_politely_asked = false);
        void rotate_r(bool is_politely_asked = false);
        void rotate_l(bool is_politely_asked = false);
};
}
