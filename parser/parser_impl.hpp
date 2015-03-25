#include "parser.h"

namespace microc {

parser_exception::parser_exception(int line, const std::string& matched):
    line_(line), matched_(matched)
{
    std::stringstream ss;
    ss << "error line " << line;

    if(matched.empty()) {
        ss << ", unexpected end of file";
    }
    else {
        ss << ", unexpected token \"" << matched << "\"";
    }

    description_ = ss.str();
}

const char* parser_exception::what() const noexcept {
    return description_.c_str();
}

int parser_exception::line() const noexcept {
    return line_;
}

const std::string& parser_exception::matched() const noexcept {
    return matched_;
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

int Parser::sanitizeIntegerToken(const std::string& str) {
    if(str.length() >= 3 && str[0] == '0' && str[1] == 'x') {
        return std::strtol(str.substr(2).c_str(), NULL, 16);
    }
    else if(str.length() >= 3 && str[0] == '0' && str[1] == 'b') {
        return std::strtol(str.substr(2).c_str(), NULL, 2);
    }
    else {
        return std::atoi(str.c_str());
    }
}

char Parser::sanitizeCharacterToken(const std::string& str) {
    if(str == "'\\0'") {
        return '\0';
    }
    else if(str == "'\\n'") {
        return '\n';
    }
    else if(str == "'\\r'") {
        return '\r';
    }
    else if(str == "'\\t'") {
        return '\t';
    }
    else if(str == "'\\''") {
        return '\'';
    }
    else {
        return str[1];
    }
}

std::string Parser::sanitizeStringToken(const std::string& str) {
    std::string val;
    val.reserve(str.length() - 2);

    for(std::size_t i = 1; i < str.length() - 1; ++i) {
        if(str[i] == '\\' && i + 1 < str.length() - 1) {
            ++i;
            if(str[i] == 'n') {
                val += '\n';
            }
            else if(str[i] == 'r') {
                val += '\r';
            }
            else if(str[i] == 't') {
                val += '\t';
            }
            else {
                val += str[i];
            }
        }
        else {
            val += str[i];
        }
    }

    return val;
}

} // namespace microc
