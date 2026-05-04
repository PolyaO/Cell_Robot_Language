#pragma once
#include "winutil/windows/base-window.hpp"

namespace Winutil {

/// @brief Window wich implements base serial output protocol.
class WindowOutput : public BaseWindow {
  public:
    WINDOW_CONSTRUCTOR(WindowOutput)

    /// @brief put string at the write position
    void write(std::wstring_view);

    /// @brief set cursor and put string at position set
    void write(engine::WindowPos pos, std::wstring_view);

    void clear() override;

  private:
    void write_line(std::wstring_view);
    void write_char(wchar_t);
    void insert_color(std::wstring_view);
    void newline();

    engine::WindowPos _cursor = {};
};

} // namespace Winutil
