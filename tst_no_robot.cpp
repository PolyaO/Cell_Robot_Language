#include <clocale>
#include <codecvt>
#include <iostream>
#include <string>
#include <string_view>
#include <variant>

#include "backend/rvals/rval.hpp"
#include "backend/rvals/var/var.hpp"
#include "interpreter/driver.hpp"
#include "winutil/engine/syntax-highlighter.hpp"
#include "winutil/main-window.hpp"
#include "winutil/window-file-view.hpp"
#include "winutil/window-output.hpp"
#include "winutil/windows-column.hpp"
#include "winutil/windows-row.hpp"

// GIRLIE PALLETTE
#define LIGHT_BLUE COLOR_RGB(179, 222, 226)
#define PINK_FROST COLOR_RGB(239, 207, 227)
#define ZEMLINIKA COLOR_RGB(226, 115, 150)
#define PINK_MIST COLOR_RGB(235, 154, 178)
#define BEIGE COLOR_RGB(236, 242, 216)

// SUMMER PALLETTE
#define LEMONADE COLOR_RGB(242, 214, 161)
#define LIMONCH COLOR_RGB(241, 168, 5)

void print_variable(const var::var_type &a, std::wstring_view var,
                    Winutil::WindowOutput &w) {
    if (std::holds_alternative<var::Var<int>>(a)) {
        w.write(L"IntVar ");
    } else
        w.write(L"BoolVar ");
    std::visit(
        [&](auto &v) {
            auto val = v.get_val();
            auto dim = v.get_dim();
            w.write(var);
            w.write(L"\n");
            w.write(L"val: ");
            std::visit(
                [&](auto &mx) {
                    for (auto &e : mx) {
                        w.write(std::to_wstring(e));
                        w.write(L", ");
                    }
                    w.write(L"\n");
                },
                val);
            w.write(L"dim: ");
            for (auto &e : dim) {
                w.write(std::to_wstring(e));
                w.write(L", ");
            }
            w.write(L"\n");
        },
        a);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./parse <maze_filename> <program_filename>"
                  << std::endl;
        return 1;
    }
    driver::Driver drv;
    unsigned lineno = drv.initialize(argv[1], argv[2]);
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    std::setlocale(LC_ALL, "en_US.utf8");
    std::locale::global(std::locale(""));  // установка системной локали
    std::wcout.imbue(std::locale());  // для вывода wstring
    std::wcin.imbue(std::locale());

    Winutil::MainWindow main_w(std::wcout, Winutil::MainWindow::max_width(),
                               Winutil::MainWindow::max_height() - 10);

    auto &main_row_w = main_w.make_window<Winutil::WindowsRow>();
    auto &debug_w = main_row_w.make_window<Winutil::WindowOutput>();
    auto &right_side_w = main_row_w.make_window<Winutil::WindowsColumn>();
    auto &robot_w = right_side_w.make_window<Winutil::WindowOutput>();
    auto &file_w = right_side_w.make_window<Winutil::WindowFileView>();

    using namespace Winutil::engine;

    Winutil::engine::SyntaxHighlighter standard_highlight(
        {
            {Pattern::word(L"struct|char_type|char|wchar_t|void|uint32_t|"
                           L"int|size_t|bool|auto"),
             COLOR(LIMONCH, BLACK)},
            {Pattern::word(L"const_cast|const|for|if|else|while|do|static"),
             COLOR(ZEMLINIKA, BLACK)},
            {Pattern::word(L"noexcept|this|operator|using"),
             COLOR(BEIGE, BLACK)},
            {Pattern::wild(L"#include|#pragma|#define"),
             COLOR(PINK_FROST, BLACK)},
            {Pattern::word(L"namespace|static_assert"), COLOR(BEIGE, BLACK)},
        },
        COLOR(LIGHT_BLUE, BLACK));

    file_w.set_highlighter(standard_highlight);
    file_w.open(argv[2]);

    debug_w.write(L"DEBUG WINDOW HERE!\n");
    robot_w.write(L"ROBOT HERE!\n");
    file_w.select({lineno - 1, 0}, {lineno - 1, Winutil::MainWindow::max_width() / 2});
    main_w.update();
    file_w.select({lineno - 1, 0}, {lineno - 1, Winutil::MainWindow::max_width() / 2});

    while (true) {
        std::wstring prompt = L"> ";
        std::wstring line;
        std::wcout << prompt;
        std::getline(std::wcin, line);
        if (line == L"n") {
            auto next_l = drv.exec_next();
            if (!next_l)
                return 0;
            else {
                debug_w.write(std::to_wstring(lineno));
                debug_w.write(L" successfully executed\n");

                lineno = next_l;
            }

        } else if (line == L"q") {
            return 0;
        } else if (line == L"s") {
            file_w.scroll(1);
        } else {
            auto var = drv.get_var(converter.to_bytes(line));
            if (!var)
                std::wcout << L"no such variable!";
            else {
                print_variable(var.value(), line, debug_w);
            }
        }
        file_w.select({lineno - 1, 0},
                      {lineno - 1, Winutil::MainWindow::max_width() / 2});
        main_w.update();
        file_w.select({lineno - 1, 0},
                      {lineno - 1, Winutil::MainWindow::max_width() / 2});
    }
}
