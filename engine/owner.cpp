#include <vector>
#include <memory>

#include "value.hpp"

using namespace std;

// Append a new Value to the store, return the pointer
// Because we are using make_unique, we don't have to worry about freeing the memory after it is used, will be handled automatically
Value& Owner::create(double v, string n) {
    store.push_back(make_unique<Value>(v, n, this));

    return *(store.back().get());
}