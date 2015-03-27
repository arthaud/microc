#include "scanner.h"
#include "../ast.hpp"
#include "../parser/parserbase.h"

#include <sstream>

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

    what_ = ss.str();
}

const char* scanner_exception::what() const noexcept {
    return what_.c_str();
}

} // namespace microc
