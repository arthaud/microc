#include "ast.hpp"
#include "parser/parser.h"

#include <fstream>
#include <iostream>


namespace ast = microc::ast;

namespace result {
enum code_t {
    success = 0,
    missing_argument_error,
    no_such_file_error,
    parse_error
};
}

int compile(std::istream& in, std::ostream& out) {
    int success;
    microc::Parser parser(in);

    try {
        success = parser.parse();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return result::parse_error;
    }

    if(success != 0) {
        std::cerr << "syntax error" << std::endl;
        return result::parse_error;
    }

    ast::Program& prog = parser.prog();
    out << "parsed:" << std::endl << prog << std::endl;

    return result::success;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cerr << "usage: " << argv[0] << " FILE" << std::endl
                  << "error: too few arguments" << std::endl;
        return result::missing_argument_error;
    }

    std::ifstream f(argv[1]);

    if(!f.is_open()) {
        std::cerr << "usage: " << argv[0] << " FILE" << std::endl
                  << "error: no such file or directory" << std::endl;
        return result::no_such_file_error;
    }

    return compile(f, std::cout);
}
