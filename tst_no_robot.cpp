#include <clocale>
#include <iostream>

#include "interpreter/driver.hpp"
#include "winutil/engine/syntax-highlighter.hpp"
#include "winutil/main-window.hpp"
#include "winutil/window-file-view.hpp"
#include "winutil/window-output.hpp"
#include "winutil/windows-column.hpp"
#include "winutil/windows-row.hpp"
#include <iostream>
#include <string>
#include <string_view>

// GIRLIE PALLETTE
#define LIGHT_BLUE COLOR_RGB(179, 222, 226)
#define PINK_FROST COLOR_RGB(239, 207, 227)
#define ZEMLINIKA COLOR_RGB(226, 115, 150)
#define PINK_MIST COLOR_RGB(235, 154, 178)
#define BEIGE COLOR_RGB(236, 242, 216)

// SUMMER PALLETTE
#define LEMONADE COLOR_RGB(242, 214, 161)
#define LIMONCH COLOR_RGB(241, 168, 5)

int main(int argc, char *argv[]) {
    //    if (argc != 3) {
    //        std::cout << "Usage: ./parse <maze_filename> <program_filename>"
    //        << std::endl; return 1;
    //    }
    //    driver::Driver drv;
    //    drv.initialize(argv[1], argv[2]);

    std::setlocale(LC_ALL, "en_US.utf8");
    std::locale::global(std::locale(""));  // установка системной локали
    std::wcout.imbue(std::locale());       // для вывода wstring
    std::wcin.imbue(std::locale());

    Winutil::MainWindow main_w(std::wcout, Winutil::MainWindow::max_width(),
                               Winutil::MainWindow::max_height() - 2);

    auto &main_row_w = main_w.make_window<Winutil::WindowsRow>();
    //auto &left_side_w = main_row_w.make_window<Winutil::WindowFileView>();
    //auto &left_side_w = main_row_w.make_window<Winutil::WindowsColumn>();
    auto &right_side_w = main_row_w.make_window<Winutil::WindowsColumn>();
    auto &robot_w = right_side_w.make_window<Winutil::WindowOutput>();
    auto &robot_w1 = right_side_w.make_window<Winutil::WindowOutput>();
    auto &robot_w2 = right_side_w.make_window<Winutil::WindowOutput>();
    auto &file_w = right_side_w.make_window<Winutil::WindowFileView>();
    auto &debug_w = main_row_w.make_window<Winutil::WindowOutput>();

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
file_w.open("../tst_no_robot.cpp");

//debug_w.write(L"DEBUG WINDOW HERE!\n");
//debug_w.write(L"DEBUG WINDOW HERE!\n");
debug_w.write(L"DEBUG WINDOW HERE!\n");
robot_w.write(L"ROBOT HERrrrrrrE!");
robot_w1.write(L"ROBOT HERE!");
robot_w2.write(L"ROBOT HERE      tdgyetgfyetfy!");
main_w.update();
while (true) {
    std::wstring prompt = L"> ";
    std::wstring line;
    std::wcout << prompt;
    std::getline(std::wcin, line);
  //  debug_w.write(line);
    robot_w.write(line);
    file_w.scroll(1);
    main_w.update();
}
}
