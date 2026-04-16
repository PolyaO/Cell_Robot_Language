#include "winutil/engine/draw-area.hpp"
#include "winutil/main-window.hpp"
#include <asm-generic/ioctls.h>
#include <ostream>
#include <ranges>
#include <stdexcept>
#include <sys/ioctl.h>
#include <unistd.h>

namespace Winutil {

MainWindow::MainWindow(std::wostream &out, unsigned width, unsigned height) :
    main_area(width, height), out(out) {}

unsigned MainWindow::max_width() noexcept {
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

unsigned MainWindow::max_height() noexcept {
    winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_row;
}

const engine::DrawArea &MainWindow::get_area() const noexcept {
    return _main->get_area();
}

void MainWindow::clear() { _main->clear(); }

void MainWindow::update() {
    _main->update();
    out << L"\ec" << COLOR_NONE;
    for (auto i : std::views::iota(0u, _main->get_area().get_info().height)) {
        std::wstring_view color = COLOR_NONE;
        for (auto c : main_area.get_line(i)) {
            if (c.get_color() != color) {
                color = c.get_color();
                out << color;
            }
            out << c.ch;
        }
        out << COLOR_NONE << L"\n";
    }
    out.flush();
}

WindowInterface &MainWindow::get_child(unsigned idx) {
    return _main->get_child(idx);
}

void MainWindow::move(engine::DrawArea &&new_area) {
    throw std::logic_error("MainWindow::move is not a valid call");
}

void MainWindow::resize(unsigned width, unsigned height) {
    main_area.resize(width, height);
    _main->move(main_area.make_area());
}

}; // namespace Winutil
