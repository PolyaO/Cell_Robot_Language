#include "winutil/basic-window.hpp"
#include "winutil/engine/draw-area.hpp"
#include "winutil/window.hpp"
#include <ranges>
#include <stdexcept>

namespace Winutil {

BasicWindow::BasicWindow(engine::DrawArea &&area) : area(std::move(area)) {}

const engine::DrawArea &BasicWindow::get_area() const noexcept { return area; }

void BasicWindow::clear() {
    auto desc = area.get_info();
    for (auto line : std::views::iota(0u, desc.height)) {
        for (auto &chr : area.get_line(line)) {
            chr.set_char(WINUTIL_EMPTY_CHAR);
        }
    }
}

WindowInterface &BasicWindow::get_child(unsigned) {
    throw std::runtime_error("Can't get child of basic window");
}

void BasicWindow::move(engine::DrawArea &&new_area) {
    area = std::move(new_area);
}

}; // namespace Winutil

