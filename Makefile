# ───────────────  cvrp-heuristics / Makefile  ──────────────────────────
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude
LDFLAGS  :=

# ----------------------------------------------------------------------
#  1)  normal CLI build
# ----------------------------------------------------------------------
SRC_CORE := $(wildcard src/*.cpp) \
            $(wildcard src/*/*.cpp) \
            $(wildcard src/*/*/*.cpp)

SRC_APP  := $(wildcard apps/*.cpp)          # CLI driver(s) with main()

OBJ_CORE := $(SRC_CORE:.cpp=.o)
OBJ_APP  := $(SRC_APP:.cpp=.o)

TARGET   := bin/cvrp-cli

all: $(TARGET)

$(TARGET): $(OBJ_CORE) $(OBJ_APP) | bin
	$(CXX) $^ $(LDFLAGS) -o $@

bin:
	@mkdir "bin" 2>NUL || exit 0

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-del /Q $(OBJ_CORE) $(OBJ_APP) tests/*.o bin/* 2>NUL

.PHONY: all clean

# ─────────────────────────  unit tests  ─────────────────────────
TEST_SRC := $(wildcard tests/*.cpp)
TEST_OBJ := $(TEST_SRC:.cpp=.o)
TEST_EXE := $(patsubst tests/%.cpp, bin/test-%, $(TEST_SRC))

tests/%.o: tests/%.cpp
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

# link each test as its own exe (no apps/main.o ⇒ no duplicate main)
bin/test-%: tests/%.cpp $(OBJ_CORE) | bin
	$(CXX) $(CXXFLAGS) -Iinclude $< $(OBJ_CORE) -o $@

# helper target that runs one exe and stops on failure
run-%: bin/test-%
	@echo "Running $<"; \
	 $<

# 'test' depends on every run-XXX helper; if any fails make stops
test: $(patsubst bin/%,run-%,$(TEST_EXE))
	@echo "All tests passed."

.PHONY: test $(patsubst bin/%,run-%,$(TEST_EXE))

