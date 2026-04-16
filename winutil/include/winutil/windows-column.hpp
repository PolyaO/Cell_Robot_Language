#pragma once
#include <memory>
#include <winutil/window.hpp>

namespace Winutil {

class WindowsColumn : public WindowInterface {
  public:
    WindowsColumn(engine::DrawArea &&area);
    WindowsColumn(const WindowsColumn &) = delete;
    WindowsColumn(WindowsColumn &&) = default;

    void clear() override;
    void move(engine::DrawArea &&new_area) override;
    void update() override;

    const engine::DrawArea &get_area() const noexcept override;
    WindowInterface &get_child(unsigned) override;

    void resize_windows(std::initializer_list<unsigned> sizes);

    template <class Iter>
        requires std::input_iterator<Iter>
    void resize_windows(Iter begin, Iter end);

    /// @brief create child window of type _Win
    template <typename _Win>
        requires(std::derived_from<_Win, WindowInterface>)
    _Win &make_window();

  private:
    void clear_rulers();
    unsigned place_windows(unsigned size);
    unsigned place_windows(const std::vector<unsigned> &sizes);
    unsigned place_window(unsigned win_id, unsigned pos, unsigned size);

    engine::DrawArea area;
    std::vector<std::unique_ptr<WindowInterface>> _rows;
};

template <class Iter>
    requires std::input_iterator<Iter>
void WindowsColumn::resize_windows(Iter begin, Iter end) {
    if (_rows.size() == 0) return;

    std::vector<unsigned> sizes(begin, end);
    if (sizes.size() > _rows.size()) sizes.resize(_rows.size());
    else if (sizes.size() < _rows.size()) {
        int start_idx = sizes.size(), end_idx = _rows.size();
        sizes.resize(_rows.size());
        for (int i = start_idx; i < end_idx; ++i) {
            sizes[i] = _rows[i]->get_area().get_info().height;
        }
    }

    engine::normalize_window_size_sequence(sizes, area.get_info().height);

    place_windows(sizes);
}

template <typename _Win>
    requires(std::derived_from<_Win, WindowInterface>)
_Win &WindowsColumn::make_window() {
    std::unique_ptr<_Win> res_ptr;
    if (_rows.size() == 0) {
        res_ptr = std::make_unique<_Win>(area.copy());
    } else {
        unsigned full_height = area.get_info().height;
        unsigned full_width = area.get_info().width;
        unsigned win_height = full_height / (_rows.size() + 1);
        if (win_height < WINDOW_MIN_SIZE)
            throw std::runtime_error("Space for windows ran out!");
        unsigned pos = place_windows(win_height - 1);
        res_ptr = std::make_unique<_Win>(
            std::move(area.subarea({pos, 0}, full_width, full_height - pos))
        );
    }
    auto &res = *res_ptr;
    _rows.push_back(std::move(res_ptr));
    return res;
}

}
