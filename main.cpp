#include <iostream>
#include "parser/include/parse_driver.hpp"

int main (int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./parse <filename>" << std::endl;
        return 1;
    }
    std::string f = argv[1];
    ParseDriver drv;
    return drv.parse(f);
}
