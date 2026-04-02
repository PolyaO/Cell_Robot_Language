#pragma once
#include "winutil/engine/draw-area.hpp"

namespace Winutil {

/// @briaef Comon drawing window interaface
class WindowInterface {
  public:
    virtual ~WindowInterface() = default;

    /// @brief Call this method to clear all window content at next draw
    virtual void clear() = 0;

    /// @brief get draw area of this window
    virtual const engine::DrawArea &get_area() const noexcept = 0;

    /// @brief Get subwindow of this window by ID
    ///
    /// Common way to get subwindows of windows of every type.
    /// Drawer never calles this method.
    virtual WindowInterface &get_child(unsigned) = 0;

    /// @brief Change the area of this window content
    virtual void move(engine::DrawArea &&new_area) = 0;

    /// @brief Screen update signal
    ///
    /// Every time this method called Screen-Drawer updates content of the
    /// screen. Every windows is allowed to change content of it's DrawArea
    /// either in this method or everywhere else outside it. However this method
    /// is somehow a callback, that screen content would be updated at the
    /// moment
    virtual void update() = 0;
};

}; // namespace Winutil
