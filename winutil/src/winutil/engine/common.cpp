#include "winutil/engine/common.hpp"
#include "winutil/engine/color-string.hpp"
#include "winutil/engine/colors.hpp"
#include <cassert>
#include <cstring>
#include <cuchar>
#include <cwchar>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string_view>

namespace Winutil::engine {

void normalize_window_size_sequence(
    std::vector<unsigned> &sizes, unsigned max_size
) {
    // if some sizes is less than minimal required, enlarge it
    for (unsigned &size : sizes) size = std::max(WINDOW_MIN_SIZE, size);

    // calculate the whole size of sequence. Adjust it with windows_num - 1
    // chars for windows delimiters
    unsigned expected_size =
        std::accumulate(sizes.begin(), sizes.end(), sizes.size() - 1);

    // if expected size if less than required, just enlarging last window
    if (expected_size < max_size) sizes.back() += max_size - expected_size;

    // in case the size is more than required, recalculation is needed
    else if (expected_size > max_size) {
        // find out the number of excess chracters
        unsigned excess = expected_size - max_size;

        // start decreasing sizes from the last one
        for (auto &size : std::ranges::reverse_view(sizes)) {
            if (excess >= size - WINDOW_MIN_SIZE) {
                excess -= size - WINDOW_MIN_SIZE;
                size = WINDOW_MIN_SIZE;
            } else {
                size -= excess;
                break;
            }
        }
    }
}

std::wstring from_utf8(std::string str) {
    mbstate_t state{};
    char32_t c32;
    const char *ptr = str.c_str(), *end = str.c_str() + str.size() + 1;
    std::wstring res = L"";

    while (std::size_t rc = std::mbrtoc32(&c32, ptr, end - ptr, &state)) {
        if (rc == (size_t)-1) break;
        if (rc == (size_t)-2) break;
        if (rc == (size_t)-3) break;
        ptr += rc;
        res += c32;
    }
    return res;
}

ColoredChar
invert_color(ColoredChar c, std::wstring_view ncl_inverse, bool force) {
    std::wstring_view color = c.get_color();
    ColoredChar res = c;

    if (res.get_color().empty()) {
        res.set_color(ncl_inverse);
        return res;
    } else if (res.get_color() == ncl_inverse) {
        res.set_color(COLOR_NONE);
        return res;
    } else if (res.get_color() == COLOR_NONE) {
        res.set_color(ncl_inverse);
        return res;
    }

    if (color.size() < 3 || color[0] != '\033' || color[1] != '['
        || color.back() != 'm') {
        return res;
    }

    std::wstring str = std::wstring(color.substr(2, color.size() - 3));
    std::wstringstream ss;
    ss << L"\033[";

    int skip_params = 0;
    while (!str.empty()) {
        switch (str[0]) {
        case '2':
            // rgb colorscheme of the already reversed color
            ss << str[0];
            skip_params = 3;
            break;
        case '5':
            // 256-color scheme of the already reversed color
            ss << str[0];
            skip_params = 1;
            break;
        case '3':
            // default text fg color - change to bg color
            ss << L'4' << str[1];
            skip_params = 0;
            break;
        case '4':
            // default text bg color - change to fg color
            ss << L'3' << str[1];
            skip_params = 0;
            break;
        case '9':
            // light text fg color - change to light bg color
            ss << L'1' << L'0' << str[1];
            skip_params = 0;
            break;
        case '1':
            // light text bg color - change to light fg color
            ss << L'9' << str[2];
            skip_params = 0;
            break;
        }

        auto semicol_pos = str.find(CTRL_DELIM);
        if (semicol_pos != std::wstring_view::npos) {
            str = str.substr(semicol_pos + 1);
            ss << CTRL_DELIM;
        } else break;
        while (skip_params--) {
            semicol_pos = str.find(CTRL_DELIM);
            if (semicol_pos != std::wstring_view::npos) {
                ss << str.substr(0, semicol_pos + 1);
                str = str.substr(semicol_pos + 1);
            } else {
                ss << str;
                str = L"";
            }
        }
    }

    ss << L'm';
    res.set_color(ss.str());

    return res;
}

void invert_color(color_string_view str, std::wstring_view ncl_inverse) {
    for (auto &ch : str) ch.set(invert_color(ch, ncl_inverse));
}

}; // namespace Winutil::engine
