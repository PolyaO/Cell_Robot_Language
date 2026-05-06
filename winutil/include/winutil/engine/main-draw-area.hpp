#pragma once
#include "winutil/engine/color-string.hpp"
#include "winutil/engine/common.hpp"
#include "winutil/engine/draw-area.hpp"
#include <vector>

namespace Winutil::engine {

class MainDrawArea {
  public:
    using lines_t = std::vector<color_string>;
    using iterator = lines_t::iterator;
    using const_iterator = lines_t::const_iterator;

    MainDrawArea(unsigned width, unsigned height);

    DrawArea make_area();
    MainDrawArea copy();

    WindowDesc get_info() const noexcept { return _desc; }

    const_iterator begin() const noexcept { return _lines.begin(); }
    const_iterator end() const noexcept { return _lines.end(); }

    iterator begin() noexcept { return _lines.begin(); }
    iterator end() noexcept { return _lines.end(); }

    void resize(unsigned width, unsigned height);

    color_string_view get_line(unsigned line_no);

  private:
    std::vector<color_string> _lines;
    WindowDesc _desc;
};

}; // namespace Winutil::engine

