#include "parser.h"

namespace microc
{

parser_exception::parser_exception(int line, const std::string& matched):
    line(line), matched(matched)
{
    std::stringstream ss;
    ss << "error line " << line << ", unexpected token \"" << matched << "\"";
    description = ss.str();
}

const char* parser_exception::what() const noexcept {
    return description.c_str();
}

int parser_exception::get_line() const noexcept {
    return line;
}

const std::string& parser_exception::get_matched() const noexcept {
    return matched;
}

Parser::Parser(std::istream &in): d_scanner(in)
{}

ast::Program& Parser::prog() {
    return d_prog;
}

inline void Parser::error(const char*) {
    throw parser_exception(d_scanner.lineNr(), d_scanner.matched());
}

inline int Parser::lex() {
    return d_scanner.lex();
}

inline void Parser::print() {
    print__();           // displays tokens if --print was specified
}

inline void Parser::exceptionHandler__(const std::exception&) {
    throw;              // re-implement to handle exceptions thrown by actions
}

std::string Parser::sanitizeStringToken(const std::string& str) {
    std::string val = str.substr(1, str.length() - 2);
    return val;
}

}
