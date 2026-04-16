#pragma once
#include <winutil/window.hpp>

#include "winutil/basic-window.hpp"

namespace Winutil {

/// @brief window wich implements basic serial output protocol.
class WindowOutput : public BasicWindow {
   public:
    WindowOutput(Winutil::engine::DrawArea &&area)
        : BasicWindow(std::move(area)) {}

    /// @brief put string at the write position
    void write(std::wstring_view);

    /// @brief set cursor and put string at position set
    void write(engine::WindowPos pos, std::wstring_view);

    void clear() override;

   private:
    void update_line(std::wstring_view);
    void insert_color(std::wstring_view);
    void newline();

    engine::WindowPos _cursor = {};
};

}  // namespace Winutil
