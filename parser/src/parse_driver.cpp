#include "parse_driver.hpp"
#include "parser.hpp"
#include "lexer.hpp"

void ParseDriver::scan_begin() {
    yyset_debug(trace_scanning);
    if (file.empty() || file == "-")
        throw std::runtime_error("no file to parse");

    else if (!(yyin = fopen(file.c_str(), "r"))) {
        std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
}

void ParseDriver::scan_end() { fclose(yyin); }

int ParseDriver::parse(const std::string &filename,
                        bool trace_scanning,
                        bool trace_parsing) {
    file = filename;
    ast.loc.initialize(&file);
    scan_begin();
    yy::parser parse(ast);
    parse.set_debug_level(trace_parsing);
    int res = parse();
    scan_end();
    return res;
}

void yy::parser::error(const yy::location& loc, const std::string& msg) {
    std::cerr << loc << ": " << msg << std::endl;
}

