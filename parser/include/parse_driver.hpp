#pragma once

#include <string>

#include "ast_maker.hpp"

class ParseDriver {
   public:
    int parse(const std::string &filename, bool trace_scanning = false,
               bool trace_parsing = false);

   private:
    bool trace_scanning = false;
    bool trace_parsing = false;
    AstMaker ast;
    std::string file;
    void scan_begin();
    void scan_end();
};
