#include "winutil/engine/main-draw-area.hpp"
#include "winutil/engine/color-string.hpp"
#include "winutil/engine/common.hpp"
#include <algorithm>
#include <format>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace Winutil::engine {

MainDrawArea::MainDrawArea(unsigned width, unsigned height) {
    resize(width, height);
}

DrawArea MainDrawArea::make_area() {
    std::vector<color_string_view> area_vect(_lines.size());
    for (auto &&[view, str] : std::ranges::zip_view(area_vect, _lines)) {
        view = str;
    }
    return DrawArea({0, 0}, std::move(area_vect));
}

void MainDrawArea::resize(unsigned width, unsigned height) {
    if (width == 0 || height == 0)
        throw std::logic_error(
            std::format("Can't initialize area with sides {}:{}", width, height)
        );
    _lines.resize(height);
    for (auto &line : _lines) line.assign(width, ColoredChar(WINUTIL_EMPTY_CHAR));
    _desc.height = _lines.size();
    _desc.width = _lines[0].size();
}

MainDrawArea MainDrawArea::copy() {
    MainDrawArea res(_desc.width, _desc.height);
    for (auto &&[line, res_line] : std::views::zip(*this, res)) {
        std::copy(line.begin(), line.end(), res_line.begin());
    }
    return std::move(res);
}

color_string_view MainDrawArea::get_line(unsigned line_no) {
    if (line_no >= _desc.height)
        throw std::out_of_range(
            std::format("line_no: {} >= {}", line_no, _desc.height)
        );
    return _lines[line_no];
}

}; // namespace Winutil::engine
