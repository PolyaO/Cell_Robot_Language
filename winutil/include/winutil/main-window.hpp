#pragma once
#include <memory>
#include <winutil/window.hpp>

namespace Winutil {

class MainWindow : public WindowInterface {
  public:
    MainWindow(std::wostream &out, unsigned width, unsigned height);
    MainWindow(const MainWindow &) = delete;
    MainWindow(MainWindow &&) = default;

    void clear() override;
    void move(engine::DrawArea &&new_area) override;
    void update() override;

    const engine::DrawArea &get_area() const noexcept override;
    WindowInterface &get_child(unsigned) override;

    /// @brief create child window of type _Win
    ///
    /// This method must be called only once for MainWindow
    template <typename _Win>
        requires(std::derived_from<_Win, WindowInterface>)
    _Win &make_window();

    /// @brief returns maximum width available for this terminal
    static unsigned max_width() noexcept;

    /// @brief returns maximum height available for this terminal
    static unsigned max_height() noexcept;

    void resize(unsigned width, unsigned height);

  private:
    std::wostream &out;
    std::unique_ptr<WindowInterface> _main;
    engine::MainDrawArea main_area;
};

template <typename _Win>
    requires(std::derived_from<_Win, WindowInterface>)
_Win &MainWindow::make_window() {
    if (_main != nullptr)
        throw std::runtime_error("Only one window could be cateated into main");
    auto res_ptr = std::make_unique<_Win>(std::move(main_area.make_area()));
    auto &res = *res_ptr;
    _main = std::move(res_ptr);
    return res;
}

}; // namespace Winutil
