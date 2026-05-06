#pragma once
#include "winutil/window.hpp"
#include "winutil/windows/base-window.hpp"

namespace Winutil {

/// @brief Window wich implements base serial output protocol.
class WindowOutput : public BaseWindow, public OutputWindow {
  public:
    WINDOW_CONSTRUCTOR(WindowOutput)

    void clear() override;

    /// @brief put string at the write position and move the cursor
    void write(std::wstring_view) override;

    /// @brief get current cursor position
    engine::WindowPos get_cursor() const noexcept override;

    WindowOutput &operator<<(std::wstring_view str) {
        write(str);
        return *this;
    }

  private:
    void write_line(std::wstring_view);
    void write_char(wchar_t);
    void insert_color(std::wstring_view);
    void newline();

    engine::WindowPos _cursor = {};
};

} // namespace Winutil
