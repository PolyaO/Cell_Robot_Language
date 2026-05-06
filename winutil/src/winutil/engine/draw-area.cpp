#include "winutil/engine/draw-area.hpp"
#include "winutil/engine/color-string.hpp"
#include "winutil/engine/common.hpp"
#include <algorithm>
#include <format>
#include <stdexcept>
#include <vector>

namespace Winutil::engine {

DrawArea DrawArea::subarea(WindowPos at, unsigned width, unsigned height) {
    if (at.col >= _desc.width || at.row >= _desc.height)
        throw std::logic_error(
            std::format(
                "Can't make subarea: pos {}:{} is out of bounds", at.col, at.row
            )
        );

    width = std::min(at.col + width, _desc.width) - at.col;
    height = std::min(at.row + height, _desc.height) - at.row;
    if (width == 0 || height == 0)
        throw std::logic_error(
            std::format("Can't initialize area with sides {}:{}", width, height)
        );

    std::vector<color_string_view> area_vect(height);
    for (int i = 0; i < height; ++i) {
        area_vect[i] = _lines[i + at.row].substr(at.col, width);
    }
    return DrawArea(_desc.global_pos + at, std::move(area_vect));
}

void DrawArea::clear() noexcept {
    for (auto &line : _lines) {
        for (auto &c: line) {
            c.set(WINUTIL_EMPTY_CHAR);
        }
    }
}

DrawArea DrawArea::copy() {
    std::vector<color_string_view> area_vect(_lines);
    return DrawArea(_desc.global_pos, std::move(area_vect));
}

color_string_view DrawArea::get_line(unsigned line_no) {
    if (line_no >= _desc.height)
        throw std::out_of_range(
            std::format("line_no:{} >= {}", line_no, _desc.height)
        );
    return _lines[line_no];
}

const color_string_view DrawArea::get_line(unsigned line_no) const {
    if (line_no >= _desc.height)
        throw std::out_of_range(
            std::format("line_no: {} >= {}", line_no, _desc.height)
        );
    return _lines[line_no];
}

DrawArea::DrawArea(WindowPos g_pos, std::vector<color_string_view> &&vect) :
    _lines(std::move(vect)) {
    _desc.global_pos = g_pos;
    _desc.height = _lines.size();
    _desc.width = _lines[0].size();
}

}; // namespace Winutil::engine
