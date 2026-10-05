# Claude wrote all this bullshit, I have no idea how makefiles work and I would live my life happily if I never have to touch them

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

SRC_DIR := engine
TEST_DIR := engine/test
NN_DIR := neural_net
BUILD_DIR := build

NN_TARGET := neural_net_bin

# All engine sources -- the reusable "library" part, shared across the
# tests and the neural net binary.
LIB_SRCS := $(wildcard $(SRC_DIR)/*.cpp)

NN_SRCS := $(wildcard $(NN_DIR)/*.cpp)

.PHONY: all clean run test nn run-nn

# Default target builds the neural net binary (engine has no main of its own)
all: nn

run: run-nn

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Usage: make test FILE=my_traversal_test
test: | $(BUILD_DIR)
ifndef FILE
	$(error Usage: make test FILE=<name-without-.cpp>, e.g. make test FILE=my_traversal_test)
endif
	$(CXX) $(CXXFLAGS) $(LIB_SRCS) $(TEST_DIR)/$(FILE).cpp -o $(BUILD_DIR)/$(FILE)_test
	./$(BUILD_DIR)/$(FILE)_test

# Builds neural_net/main.cpp + neural_net/*.cpp (Neuron/Layer/MLP, etc.)
# linked against the engine library.
nn: | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(LIB_SRCS) $(NN_SRCS) -o $(BUILD_DIR)/$(NN_TARGET)

run-nn: nn
	./$(BUILD_DIR)/$(NN_TARGET)

clean:
	rm -rf $(BUILD_DIR)
