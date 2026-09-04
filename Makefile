# Claude wrote all this bullshit, I have no idea how makefiles work and I would live my life happily if I never have to touch them

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

SRC_DIR := engine
TEST_DIR := engine/test
NN_DIR := neural_net
BUILD_DIR := build

TARGET := autograd
NN_TARGET := neural_net_bin

# All engine sources except main.cpp -- the reusable "library" part,
# shared across the real binary, tests, and now the neural net binary.
LIB_SRCS := $(filter-out $(SRC_DIR)/main.cpp,$(wildcard $(SRC_DIR)/*.cpp))
LIB_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LIB_SRCS))

MAIN_OBJ := $(BUILD_DIR)/main.o

NN_SRCS := $(wildcard $(NN_DIR)/*.cpp)

.PHONY: all clean run test nn run-nn

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
test:
ifndef FILE
	$(error Usage: make test FILE=<name-without-.cpp>, e.g. make test FILE=my_traversal_test)
endif
	$(CXX) $(CXXFLAGS) $(LIB_SRCS) $(TEST_DIR)/$(FILE).cpp -o $(BUILD_DIR)/$(FILE)_test
	./$(BUILD_DIR)/$(FILE)_test

# Builds neural_net/main.cpp + neural_net/*.cpp (Neuron/Layer/MLP, etc.)
# linked against the engine library (minus engine's own main.cpp, same
# reasoning as `test` -- avoids a duplicate 'main' at link time).
nn: $(LIB_SRCS) $(NN_SRCS)
	$(CXX) $(CXXFLAGS) $(LIB_SRCS) $(NN_SRCS) -o $(BUILD_DIR)/$(NN_TARGET)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LIB_SRCS) $(NN_SRCS) -o $(BUILD_DIR)/$(NN_TARGET)

run-nn: nn
	./$(BUILD_DIR)/$(NN_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)