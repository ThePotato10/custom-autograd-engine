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
        double              value;
        std::string         name;
        double              gradient;
        Operation           operation;
        Owner*              globalOwner;
        std::vector<Value*> parents;

        Value(double v, std::string n, Owner* ownerRef);
        std::string toString();

        Value& operator+(Value& other);
        Value& operator*(Value& other);
        Value& operator-(Value& other);
        Value& operator/(Value& other);
        Value& exp(double x);

        void backprop();
};