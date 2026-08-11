CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

SRC_DIR := engine
TEST_DIR := engine/test
BUILD_DIR := build
TARGET := autograd

# All engine sources except main.cpp -- this is the "library" part,
# shared between the real binary and any test binary.
LIB_SRCS := $(filter-out $(SRC_DIR)/main.cpp,$(wildcard $(SRC_DIR)/*.cpp))
LIB_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LIB_SRCS))

MAIN_OBJ := $(BUILD_DIR)/main.o

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(LIB_OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $(LIB_OBJS) $(MAIN_OBJ) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./$(TARGET)

# Usage: make test FILE=my_traversal_test
# Builds engine/test/my_traversal_test.cpp as the entry point, linked
# against every engine/*.cpp EXCEPT main.cpp (so there's no duplicate
# 'main' at link time), then runs the result immediately.
test:
ifndef FILE
	$(error Usage: make test FILE=<name-without-.cpp>, e.g. make test FILE=my_traversal_test)
endif
	$(CXX) $(CXXFLAGS) $(LIB_SRCS) $(TEST_DIR)/$(FILE).cpp -o $(BUILD_DIR)/$(FILE)_test
	./$(BUILD_DIR)/$(FILE)_test

clean:
	rm -rf $(BUILD_DIR) $(TARGET)