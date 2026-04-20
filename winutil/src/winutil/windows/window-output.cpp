#include "winutil/windows/window-output.hpp"
#include "winutil/engine/draw-area.hpp"
#include <ranges>
#include <stdexcept>
#include <string_view>

namespace Winutil {

void WindowOutput::update_line(std::wstring_view str) {
    auto area = get_area();
    auto desc = area.get_info();
    if (_cursor.row >= desc.height || _cursor.col >= desc.width) return;
    auto line = area.get_line(_cursor.row);
    auto to_write = str.substr(0, line.size() - _cursor.col);
    for (auto &&[line_chr, chr] : std::ranges::zip_view(
             line.substr(_cursor.col), str.substr(0, line.size() - _cursor.col)
         )) {
        line_chr.set_char(chr);
    }
    _cursor.col += to_write.size();
}

void WindowOutput::clear() {
    BaseWindow::clear();
    _cursor = {};
}

void WindowOutput::insert_color(std::wstring_view color) {
    auto area = get_area();
    auto desc = area.get_info();
    if (_cursor.row >= desc.height || _cursor.col >= desc.width) return;
    area.get_line(_cursor.row)[_cursor.col].set_color(color);
}

void WindowOutput::newline() {
    auto area = get_area();
    auto desc = area.get_info();
    if (_cursor.row == desc.height - 1) return;
    _cursor.col = 0;
    _cursor.row += 1;
}

void WindowOutput::write(std::wstring_view str) {
    auto area = get_area();
    auto desc = area.get_info();
    if (_cursor.row == desc.height) return;
    while (!str.empty()) {
        auto br_pos = str.find(L'\n');
        auto line = str.substr(0, br_pos);

        auto e_pos = line.find(L'\033');
        while (e_pos != std::string_view::npos) {
            update_line(line.substr(0, e_pos));
            line = line.substr(e_pos);
            auto m_pos = line.find(L'm');
            if (m_pos == std::string_view::npos)
                throw std::runtime_error("Invalid color escape!");
            insert_color(line.substr(0, m_pos + 1));
            line = line.substr(m_pos + 1);
            e_pos = line.find(L'\033');
        }

        update_line(line);
        if (br_pos != std::string_view::npos) {
            newline();
            str = str.substr(br_pos + 1);
        } else str = L"";
    }
}

}; // namespace Winutil
