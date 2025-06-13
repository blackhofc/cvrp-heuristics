# ───────────────  cvrp-heuristics / Makefile  ────────────────
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude
LDFLAGS  :=                       # add libs here if ever needed

# ---------------------------------------------------------------------------
#  1)  project object files
# ---------------------------------------------------------------------------
SRC_CORE := $(wildcard src/*.cpp) \
            $(wildcard src/*/*.cpp) \
            $(wildcard src/*/*/*.cpp)

SRC_APP  := $(wildcard apps/*.cpp)          # CLI driver(s) with main()

OBJ_CORE := $(SRC_CORE:.cpp=.o)
OBJ_APP  := $(SRC_APP:.cpp=.o)              # contains apps/main.o

TARGET   := bin/cvrp-cli

all: $(TARGET)

$(TARGET): $(OBJ_CORE) $(OBJ_APP) | bin
	$(CXX) $^ $(LDFLAGS) -o $@

# create bin/ if missing
bin:
	@mkdir "bin" 2>NUL || exit 0

# pattern rule for every .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-del /Q $(OBJ_CORE) $(OBJ_APP) tests/*.o bin/* 2>NUL

.PHONY: all clean

# ---------------------------------------------------------------------------
#  2)  simple ASSERT-based unit tests  (no GoogleTest)
# ---------------------------------------------------------------------------
TEST_SRC := $(wildcard tests/*.cpp)
TEST_OBJ := $(TEST_SRC:.cpp=.o)

tests/%.o: tests/%.cpp
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

# link tests WITHOUT apps/main.o  → avoids duplicate-main linker error
test: $(OBJ_CORE) $(TEST_OBJ) | bin
	$(CXX) $^ -o bin/tests
	@echo "Running tests..."
	@bin/tests && echo "All tests passed."

.PHONY: test
