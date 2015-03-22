#include "scanner.h"
#include "../ast.hpp"
#include "../parser/parserbase.h"

namespace microc {

scanner_exception::scanner_exception(int line, const std::string& matched):
    line(line), matched(matched)
{
    std::stringstream ss;
    ss << "error line " << line;

    if(matched.empty()) {
        ss << ", unexpected end of file";
    }
    else {
        ss << ", near \"" << matched << "\"";
    }

    description = ss.str();
}

const char* scanner_exception::what() const noexcept {
    return description.c_str();
}

int scanner_exception::get_line() const noexcept {
    return line;
}

const std::string& scanner_exception::get_matched() const noexcept {
    return matched;
}

}
