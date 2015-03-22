CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++14

all: microc

ast.o: ast.cpp
	$(CXX) $(CXXFLAGS) -c -o ast.o ast.cpp

scanner/lex.cc: scanner/lex.l
	flexc++ --target-directory=scanner scanner/lex.l

parser/parse.cc: parser/parse.y
	bisonc++ --target-directory=parser parser/parse.y

scanner/lex.o: scanner/lex.cc parser/parse.cc
	$(CXX) $(CXXFLAGS) -Iscanner -c -o scanner/lex.o scanner/lex.cc

parser/parse.o: parser/parse.cc scanner/lex.cc
	$(CXX) $(CXXFLAGS) -Iparser -c -o parser/parse.o parser/parse.cc

microc: ast.o scanner/lex.o parser/parse.o microc.cpp
	$(CXX) $(CXXFLAGS) -o microc ast.o scanner/lex.o parser/parse.o microc.cpp

clean:
	rm -f scanner/lex.cc scanner/scannerbase.h
	rm -f parser/parse.cc parser/parserbase.h
	rm -f *.o */*.o microc
