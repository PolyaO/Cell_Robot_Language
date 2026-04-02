#pragma once
#include "winutil/engine/color-string.hpp"
#include "winutil/engine/common.hpp"
#include <vector>

namespace Winutil::engine {

class DrawArea {
  public:
    DrawArea() = default;

    DrawArea subarea(WindowPos at, unsigned width, unsigned height);
    DrawArea copy();

    color_string_view get_line(unsigned line_no);
    const color_string_view get_line(unsigned line_no) const;

    WindowDesc get_info() const noexcept { return _desc; }

  private:
    DrawArea(std::vector<color_string_view> &&);
    friend class MainDrawArea;

    std::vector<color_string_view> _lines;
    WindowDesc _desc;
};

class MainDrawArea {
  public:
    MainDrawArea(unsigned width, unsigned height);

    DrawArea make_area();

    void resize(unsigned width, unsigned height);

    color_string_view get_line(unsigned line_no);

  private:
    std::vector<color_string> _lines;
    WindowDesc _desc;
};

}; // namespace Winutil::engine
