#include "scanner.h"
#include "../ast.hpp"
#include "../parser/parserbase.h"

namespace microc {

scanner_exception::scanner_exception(int line, const std::string& matched):
    line_(line), matched_(matched)
{
    std::stringstream ss;
    ss << "error line " << line;

    if(matched.empty()) {
        ss << ", unexpected end of file";
    }
    else {
        ss << ", near \"" << matched << "\"";
    }

    description_ = ss.str();
}

const char* scanner_exception::what() const noexcept {
    return description_.c_str();
}

int scanner_exception::line() const noexcept {
    return line_;
}

const std::string& scanner_exception::matched() const noexcept {
    return matched_;
}

}
