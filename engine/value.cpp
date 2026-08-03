#include <iostream>
#include <vector>
#include <string>

#include "value.hpp"
#include "owner.hpp"

using namespace std;

Value::Value(double v, string n, Owner* ownerRef) : 
    value(v), 
    name(n), 
    gradient(0.0), 
    operation(Operation::LEAF), 
    globalOwner(ownerRef) {}

string Value::toString() {
    string s = "Value " + name + "\ndata: " + to_string(value) + " | " "grad: " + to_string(gradient) + "\n";

    if (parents.size() > 0) {
        s += "Parents: ";
        
        for (int i = 0; i < parents.size(); ++i) {
            s += parents.at(i)->name + ", ";
        }

        s.erase(s.length() - 2);
        s += "\n";
    } 

    return s;
}

Value* Value::operator+(Value& other) {
    Value* sum = globalOwner->create(this->value + other.value, "(" + this->name + " + " + other.name + ")");
    sum->parents = vector<Value*>{this, &other};
    sum->operation = Operation::ADD;

    return sum;
}

Value* Value::operator*(Value& other) {
    Value* sum = globalOwner->create(this->value * other.value, "(" + this->name + " * " + other.name + ")");
    sum->parents = vector<Value*>{this, &other};
    sum->operation = Operation::MUL;

    return sum;
}

Value* Value::operator-(Value& other) {
    Value* sum = globalOwner->create(this->value - other.value, "(" + this->name + " - " + other.name + ")");
    sum->parents = vector<Value*>{this, &other};
    sum->operation = Operation::SUB;

    return sum;
}

Value* Value::operator/(Value& other) {
    Value* sum = globalOwner->create(this->value / other.value, "(" + this->name + " / " + other.name + ")");
    sum->parents = vector<Value*>{this, &other};
    sum->operation = Operation::DIV;

    return sum;
}