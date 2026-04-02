#pragma once
#include <winutil/window.hpp>

namespace Winutil {

/// @brief The most simple standard drawable window.
class BasicWindow: public WindowInterface {
  public:
    BasicWindow(engine::DrawArea &&area);
    BasicWindow(const BasicWindow &) = delete;
    BasicWindow(BasicWindow &&) = default;

    void clear() override;
    void move(engine::DrawArea &&new_area) override;
    void update() override {}

    const engine::DrawArea &get_area() const noexcept override;
    WindowInterface &get_child(unsigned) override;

  private:
    engine::DrawArea area;
};

}

