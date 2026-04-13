#include "winutil/window-file-view.hpp"
#include "winutil/basic-window.hpp"
#include "winutil/engine/color-string.hpp"
#include "winutil/engine/common.hpp"
#include "winutil/engine/draw-area.hpp"
#include <fstream>
#include <string>
#include <string_view>

namespace Winutil {

void WindowFileView::open(const std::string &filename) {
    std::ifstream fin(filename, std::ios::binary);
    std::string raw_line_buf;

    while (std::getline(fin, raw_line_buf)) {
        std::wstring line_buf = engine::from_utf8(raw_line_buf);
        if (!_config.syntax_highlight) {
            _lines.emplace_back(line_buf.begin(), line_buf.end());
        } else {
            _lines.push_back(_highlighter->get().highlight_line(line_buf));
        }
    }
    write_content();
}

void WindowFileView::scroll(signed nrows) noexcept {
    int new_first_line = _first_line + nrows;
    new_first_line = std::max(0, new_first_line);
    new_first_line = std::min((int)_lines.size() - 1, new_first_line);
    _first_line = new_first_line;
    write_content();
}

void WindowFileView::scroll_horizontal(signed ncols) noexcept {
    int new_first_col = _first_col + ncols;
    new_first_col = std::max(0, new_first_col);
    _first_col = new_first_col;
    write_content();
}

void WindowFileView::write_content() noexcept {
    auto desc = get_area().get_info();
    for (int line_no = 0; line_no < desc.height; ++line_no) {
        auto screen_line = get_area().get_line(line_no);
        if (line_no + _first_line >= _lines.size()) {
            for (auto &chr : screen_line) chr.set_char(WINUTIL_EMPTY_CHAR);
            continue;
        }

        auto file_line = _lines[line_no + _first_line];
        for (int i = 0; i < desc.width; ++i) {
            if (i + _first_col < file_line.size())
                screen_line[i].set(file_line[i + _first_col]);
            else screen_line[i].set_char(WINUTIL_EMPTY_CHAR);
        }

        apply_selection();
    }
}

void WindowFileView::apply_row_selection(
    unsigned row, unsigned from, unsigned to
) noexcept {
    if (from > to) {
        unsigned tmp = from;
        from = to;
        to = tmp;
    } else if (from == to) return;

    engine::invert_color(
        get_area().get_line(row).substr(from, to - from + 1)
    );
}

void WindowFileView::apply_selection() noexcept {
    auto desc = get_area().get_info();
    _select_from.col = std::min(_select_from.col, desc.width - 1);
    _select_from.row = std::min(_select_from.row, desc.height - 1);
    _select_to.col = std::min(_select_to.col, desc.width - 1);
    _select_to.row = std::min(_select_to.row, desc.height - 1);

    unsigned first_row = _select_from.row;
    unsigned last_row = _select_to.row;
    unsigned first_col = _select_from.col;
    unsigned last_col = _select_to.col;
    if (first_row == last_row) {
        if (first_col == last_col) return;
        apply_row_selection(first_row, first_col, last_col);
        return;
    } else if (first_row > last_row) {
        first_row = _select_to.row;
        last_row = _select_from.row;
        first_col = _select_to.col;
        last_col = _select_from.col;
    }
    apply_row_selection(first_row, first_col, desc.width - 1);
    for (int i = first_row + 1; i < last_row; ++i) {
        apply_row_selection(i, 0, desc.width - 1);
    }
    apply_row_selection(last_row, 0, last_col);
}

void WindowFileView::select(
    engine::WindowPos from, engine::WindowPos to
) noexcept {
    _select_from = from;
    _select_to = to;
    apply_selection();
}

void WindowFileView::set_highlighter(
    const engine::SyntaxHighlighter &highlighter
) noexcept {
    _highlighter = highlighter;
    _config.syntax_highlight = true;
}

void WindowFileView::move(engine::DrawArea &&new_area) {
   // static_cast<BasicWindow *>(this)->move(std::move(new_area));
    BasicWindow::move(std::move(new_area));
    write_content();
}

}; // namespace Winutil

