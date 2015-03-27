// Generated by Flexc++ V2.01.00 on Sat, 21 Mar 2015 18:08:45 +0100

#ifndef MICROC_SCANNER_SCANNER_H
#define MICROC_SCANNER_SCANNER_H

#include "scannerbase.h"

#include <exception>

namespace microc {

class scanner_exception : public std::exception {
    public:
        scanner_exception(int line, const std::string& matched);
        virtual const char* what() const noexcept;
        int line() const noexcept { return line_; }
        const std::string& matched() const noexcept { return matched_; }

    private:
        int line_;
        std::string matched_;
        std::string what_;
};

class Scanner: public ScannerBase {
    public:
        explicit Scanner(std::istream &in = std::cin,
                                std::ostream &out = std::cout);

        Scanner(std::string const &infile, std::string const &outfile);

        int lex();

    private:
        int lex__();
        int executeAction__(size_t ruleNr);

        void print();
        void preCode();     // re-implement this function for code that must 
                            // be exec'ed before the patternmatching starts

        void postCode(PostEnum__ type);    
                            // re-implement this function for code that must 
                            // be exec'ed after the rules's actions.
};

inline Scanner::Scanner(std::istream &in, std::ostream &out):
    ScannerBase(in, out)
{}

inline Scanner::Scanner(std::string const &infile, std::string const &outfile):
    ScannerBase(infile, outfile)
{}

inline int Scanner::lex() {
    return lex__();
}

inline void Scanner::preCode() {
    // optionally replace by your own code
}

inline void Scanner::postCode(PostEnum__) {
    // optionally replace by your own code
}

inline void Scanner::print() {
    print__();
}

} // namespace microc

#endif // MICROC_SCANNER_SCANNER_H
