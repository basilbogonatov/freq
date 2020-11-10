CXX := g++
CXXFLAGS := -Wall -Wpedantic -Werror -O2 -std=c++14 

all:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o freq main.cpp $(LDLIBS)

clean:
	rm freq

test: all
	./freq test.txt out.txt && cat out.txt 
