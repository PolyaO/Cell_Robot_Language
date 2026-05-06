#pragma once
#include <winutil/engine/draw-area.hpp>

namespace Winutil {

/// @brief Window interface
class Window {
  public:
    virtual ~Window() = default;

    /// @brief Clear content of the window at next screen redraw
    virtual void clear() = 0;

    /// @brief Change the area of where window draws content
    virtual void move(engine::DrawArea &&new_area) = 0;

    /// @brief Screen update operation
    ///
    /// Every time this method called, content of the screen is being updated.
    /// Every windows is allowed to change content of it's DrawArea
    /// either in this method or everywhere else outside it.
    virtual void update() = 0;

    /// @brief get draw area of this window
    virtual const engine::DrawArea &get_area() const noexcept = 0;
};

/// @brief Output window interface
class OutputWindow : virtual public Window {
  public:
    /// @brief put string at the write position and move the cursor
    virtual void write(std::wstring_view) = 0;

    /// @brief get current cursor position
    virtual engine::WindowPos get_cursor() const noexcept = 0;
};

} // namespace Winutil
