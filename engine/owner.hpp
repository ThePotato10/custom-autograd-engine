#pragma once
#include <vector>
#include <string>
#include <memory>

// forward declaration to prevent recursive build bugs
class Value;

// Defines a single global store for all value nodes to make memory management easier
// Further, all creating and deleting values is handled through this store
// This means that all Values are passed by pointer, which simplifies a lot of bugs around copying Values
class Owner {
    public:
        std::vector<std::unique_ptr<Value>> store;
        Value& create(double v, std::string n);
};