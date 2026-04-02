
#include "winutil/engine/syntax-highlighter.hpp"
#include "winutil/engine/color-string.hpp"
#include <regex>
#include <vector>

namespace Winutil::engine {

color_string SyntaxHighlighter::highlight_line(const std::wstring &line) const {
    color_string res = color_string(line.begin(), line.end());
    size_t cur_pos = 0;

    if (res.empty()) return res;

    std::wsregex_iterator begin(line.begin(), line.end(), _regex);
    std::wsregex_iterator end;

    for (auto it = begin; it != end; ++it) {
        std::wsmatch match = *it;
        res[cur_pos].set_color(_default_color);

        cur_pos = match.position();
        for (int group_no = 1; group_no < match.size(); ++group_no) {
            if (match[group_no].matched && (group_no - 1) < _colors.size()) {
                std::wstring_view color = _colors[group_no - 1];
                for (int i = 0; i < match.length(); ++i)
                    res[cur_pos + i].set_color(color);
                break;
            }
        }

        cur_pos += match.length();
    }
    if (cur_pos >= res.size()) cur_pos = res.size() - 1;
    res[cur_pos].set_color(_default_color);
    return res;
}

}; // namespace Winutil::engine
