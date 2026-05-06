#pragma once
#include <winutil/engine/draw-area.hpp>
#include <winutil/window.hpp>

namespace Winutil {

#define WINDOW_CONSTRUCTOR(_cls)                                            \
    _cls(Winutil::engine::DrawArea &&area) : BaseWindow(std::move(area)) {} \
    _cls(const _cls &) = delete;                                            \
    _cls(_cls &&) = default;

/// @brief Base Window class
class BaseWindow : virtual public Window {
  public:
    BaseWindow(engine::DrawArea &&area);
    BaseWindow(const BaseWindow &) = delete;
    BaseWindow(BaseWindow &&) = default;

    void clear() override;
    void move(engine::DrawArea &&new_area) override;
    void update() override;
    const engine::DrawArea &get_area() const noexcept override;

    /// @brief get window size imformation
    const engine::WindowDesc &get_size() const noexcept {
        return area.get_info();
    };

  protected:
    engine::DrawArea area;
};

} // namespace Winutil

