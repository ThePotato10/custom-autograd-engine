#pragma once
#include <vector>
#include <string>

#include "owner.hpp"

// LEAF, ADD, MUL, SUB, DIV, POWER
// LEAF is essentially the null option, used to prevent actual nullptr bugs when traversing for backprop
enum Operation {
    LEAF, ADD, MUL, SUB, DIV, POWER
};

class Value {
    public:
        std::vector<Value*> parents;
        Operation operation;
        Owner* globalOwner;
        double value;
        double gradient;
        std::string name;

        Value(double v, std::string n, Owner* ownerRef);
        std::string toString();

        Value& operator+(Value& other);
        Value& operator*(Value& other);
        Value& operator-(Value& other);
        Value& operator/(Value& other);

        void backprop();
};