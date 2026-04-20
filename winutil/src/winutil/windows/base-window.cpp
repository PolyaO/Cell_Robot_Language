#include "winutil/windows/base-window.hpp"
#include "winutil/engine/draw-area.hpp"
#include <ranges>

namespace Winutil {

BaseWindow::BaseWindow(engine::DrawArea &&area) : area(std::move(area)) {}

void BaseWindow::clear() {
    auto desc = area.get_info();
    for (auto line : std::views::iota(0u, desc.height)) {
        for (auto &chr : area.get_line(line)) {
            chr.set_char(WINUTIL_EMPTY_CHAR);
        }
    }
}

void BaseWindow::move(engine::DrawArea &&new_area) {
    area = std::move(new_area);
}

void BaseWindow::update() {}

}; // namespace Winutil

