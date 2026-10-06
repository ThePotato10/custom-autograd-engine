#include<vector>
#include<string>

#include "../../engine/owner.hpp"
#include "../../engine/value.hpp"

using namespace std;

vector<Value*> toValues(const std::vector<double>& values, Owner& owner) {
    vector<Value*> createdVals;
    
    for (size_t i = 0; i < values.size(); ++i) {
        Value* v = &owner.create(values[i], "i" + to_string(i));
        createdVals.push_back(v);
    }

    return createdVals;
}

void descend(vector<Value*>& params, double learningRate) {
    for (Value* p : params) p->value -= learningRate * p->gradient;
}