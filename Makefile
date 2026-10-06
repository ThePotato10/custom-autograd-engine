# Claude wrote all this bullshit, I have no idea how makefiles work and I would live my life happily if I never have to touch them

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

ENGINE_DIR := engine
TEST_DIR := engine/test
NN_DIR := neural_net
BUILD_DIR := build

TARGET := $(BUILD_DIR)/autograd

# main.cpp does `#include "api/api.hpp"`, and api/ lives under neural_net/,
# so neural_net/ goes on the include path.
INCLUDES := -I$(NN_DIR)

# The engine on its own -- the only thing the engine tests link against.
ENGINE_SRCS := $(wildcard $(ENGINE_DIR)/*.cpp)

# Neural net + training API. neural_net/src/main.cpp is a leftover entrypoint,
# excluded so it doesn't collide with the real main() in ./main.cpp.
NN_SRCS := $(filter-out $(NN_DIR)/src/main.cpp, \
             $(wildcard $(NN_DIR)/src/*.cpp) $(wildcard $(NN_DIR)/api/*.cpp))

MAIN_SRC := main.cpp

# Headers are listed as prerequisites so editing only a .hpp still triggers a rebuild
HEADERS := $(wildcard $(ENGINE_DIR)/*.hpp $(NN_DIR)/src/*.hpp $(NN_DIR)/api/*.hpp)

.PHONY: all run test clean

# Default target: build the training program from ./main.cpp
all: $(TARGET)

$(TARGET): $(MAIN_SRC) $(ENGINE_SRCS) $(NN_SRCS) $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(ENGINE_SRCS) $(NN_SRCS) $(MAIN_SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Engine tests, built against the engine only (no neural net code).
# Usage: make test FILE=backprop_test
test: | $(BUILD_DIR)
ifndef FILE
	$(error Usage: make test FILE=<name-without-.cpp>, e.g. make test FILE=backprop_test)
endif
	$(CXX) $(CXXFLAGS) $(ENGINE_SRCS) $(TEST_DIR)/$(FILE).cpp -o $(BUILD_DIR)/$(FILE)_test
	./$(BUILD_DIR)/$(FILE)_test

clean:
	rm -rf $(BUILD_DIR)
