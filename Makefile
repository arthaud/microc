CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++14

all: microc

ast.o: ast.cpp
	$(CXX) $(CXXFLAGS) -c -o ast.o ast.cpp

microc: ast.o microc.cpp
	$(CXX) $(CXXFLAGS) -o microc ast.o microc.cpp

clean:
	rm -f *.o microc
