#pragma once
#include "winutil/basic-window.hpp"
#include "winutil/engine/common.hpp"
#include "winutil/engine/syntax-highlighter.hpp"
#include <functional>
#include <optional>
#include <vector>

namespace Winutil {

class WindowFileView : public BasicWindow {
  public:
    struct config {
        bool line_numbers = true;
        bool syntax_highlight = false;
    };

    struct LinePos {
        unsigned line_no;
        unsigned char_no;
    };

    WindowFileView(Winutil::engine::DrawArea &&area) :
        BasicWindow(std::move(area)) {}

    void move(engine::DrawArea &&new_area) override;

    void open(const std::string &filename);

    void scroll(signed nrows) noexcept;
    void scroll_to(unsigned line, bool at_center = false) noexcept;
    void scroll_horizontal(signed ncols) noexcept;

    void set_highlighter(const engine::SyntaxHighlighter &) noexcept;

    void select(LinePos from, LinePos to) noexcept;

  private:
    void write_content() noexcept;
    void apply_selection() noexcept;
    void apply_row_selection(unsigned row, unsigned from, unsigned to) noexcept;

    engine::WindowPos linepos2winpos(LinePos pos) noexcept;

    std::vector<engine::color_string> _lines;
    unsigned _first_line = 0;
    unsigned _first_col = 0;
    unsigned _first_screen_col = 0;

    LinePos _select_from = {};
    LinePos _select_to = {};

    std::optional<std::reference_wrapper<const engine::SyntaxHighlighter>>
        _highlighter;

    config _config = {};
};

}; // namespace Winutil
