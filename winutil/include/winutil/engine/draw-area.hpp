#pragma once
#include "winutil/engine/color-string.hpp"
#include "winutil/engine/common.hpp"
#include <vector>

namespace Winutil::engine {

class DrawArea {
  public:
    using lines_t = std::vector<color_string_view>;
    using iterator = lines_t::iterator;
    using const_iterator = lines_t::const_iterator;

    DrawArea() = default;

    DrawArea subarea(WindowPos at, unsigned width, unsigned height);
    DrawArea copy();

    color_string_view get_line(unsigned line_no);
    const color_string_view get_line(unsigned line_no) const;

    color_string_view operator[](unsigned line_no) { return get_line(line_no); }
    const color_string_view operator[](unsigned line_no) const {
        return get_line(line_no);
    }

    const_iterator begin() const noexcept { return _lines.begin(); }
    const_iterator end() const noexcept { return _lines.end(); }

    iterator begin() noexcept { return _lines.begin(); }
    iterator end() noexcept { return _lines.end(); }

    void clear() noexcept;

    const WindowDesc &get_info() const noexcept { return _desc; }

  private:
    DrawArea(WindowPos g_pos, std::vector<color_string_view> &&);
    friend class MainDrawArea;

    std::vector<color_string_view> _lines;
    WindowDesc _desc;
};

}; // namespace Winutil::engine
