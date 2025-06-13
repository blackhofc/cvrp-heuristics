# cvrp-heuristics/Makefile
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude
LDFLAGS  :=

SRC := $(wildcard src/*.cpp) \
       $(wildcard src/*/*.cpp) \
       $(wildcard src/*/*/*.cpp) \
       $(wildcard apps/*.cpp)

OBJ    := $(SRC:.cpp=.o)
TARGET := bin/cvrp-cli

all: $(TARGET)

$(TARGET): $(OBJ) | bin
	$(CXX) $(OBJ) $(LDFLAGS) -o $@

bin:
	@mkdir "bin" 2>NUL || exit 0

# Pattern rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-del /Q $(OBJ) $(TARGET) 2>NUL

.PHONY: all clean
