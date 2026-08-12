#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>

#include "value.hpp"
#include "owner.hpp"
#include "traversal.hpp"

using namespace std;

Value::Value(double v, string n, Owner* ownerRef) : 
    value(v), 
    name(n), 
    gradient(0.0), 
    operation(Operation::LEAF), 
    globalOwner(ownerRef) 
{}

string Value::toString() {
    string s = "Value " + name + "\ndata: " + to_string(value) + " | " "grad: " + to_string(gradient) + "\n";

    if (parents.size() > 0) {
        s += "Parents: ";
        
        for (size_t i = 0; i < parents.size(); ++i) {
            s += parents.at(i)->name + ", ";
        }

        s.erase(s.length() - 2);
        s += "\n";
    } 

    return s;
}

Value& Value::operator+(Value& other) {
    Value* sum = &globalOwner->create(this->value + other.value, "(" + this->name + " + " + other.name + ")");
    sum->parents = vector<Value*>{this, &other};
    sum->operation = Operation::ADD;

    return *sum;
}

Value& Value::operator*(Value& other) {
    Value* sum = &globalOwner->create(this->value * other.value, "(" + this->name + " * " + other.name + ")");
    sum->parents = vector<Value*>{this, &other};
    sum->operation = Operation::MUL;

    return *sum;
}

Value& Value::operator-(Value& other) {
    Value* sum = &globalOwner->create(this->value - other.value, "(" + this->name + " - " + other.name + ")");
    sum->parents = vector<Value*>{this, &other};
    sum->operation = Operation::SUB;

    return *sum;
}

Value& Value::operator/(Value& other) {
    Value* sum = &globalOwner->create(this->value / other.value, "(" + this->name + " / " + other.name + ")");
    sum->parents = vector<Value*>{this, &other};
    sum->operation = Operation::DIV;

    return *sum;
}

Value& Value::exp(double x) {
    // This is just because I'm ocd about formatting bugs
    std::ostringstream oss;
    oss << std::setprecision(15) << x;
    std::string s = oss.str();

    Value* sum = &globalOwner->create(pow(this->value, x), "(" + this->name + "^" + s + ")");
    sum->parents = vector<Value*>{this, &(globalOwner->create(x, s))};
    sum->operation = Operation::POWER;

    return *sum;
}

void updateGradient(Value* v) {
    if (v->parents.size() > 0) { // Otherwise would segfault on leaf nodes, since v->parents[i] is null
        Value* first  = v->parents[0];
        Value* second = v->parents[1];
        double localGradient = v->gradient;

        switch(v->operation) {
            case Operation::ADD:
                first->gradient  += 1 * localGradient;
                second->gradient += 1 * localGradient;
                break;
            case Operation::MUL:
                first->gradient  += second->value * localGradient;
                second->gradient += first->value  * localGradient;
                break;
            case Operation::SUB:
                first->gradient  +=  1 * localGradient;
                second->gradient += -1 * localGradient;
                break;
            case Operation::DIV:
                // Fuck the quotient rule
                first->gradient  += (1 / second->value) * localGradient;
                second->gradient += -1 * first->value * (1 / pow(second->value, 2)) * localGradient;
                break;
            case Operation::POWER:
                first->gradient += second->value * pow(first->value, (second->value - 1)) * localGradient;
                // Don't need to touch the second gradient because the second Value is just a placeholder for traversal, doesn't actually exist as a variable
                break;
            case Operation::LEAF: // Entirely redundant bc of the check above, but makes the compiler happy
                break;
        }
    }
}

void Value::backprop() {
    vector<Value*> traversal = buildTraversal(*this); // Build out a traversal with the Value that backprop is called on as the entry
    reverse(traversal.begin(), traversal.end()); // Need reverse topological ordering for backprop

    traversal.at(0)->gradient = 1.0; // First gradient is always set to 1 (derivative of a variable wrt itself is always 1)

    for (size_t i = 0; i < traversal.size(); ++i) {
        Value* curr = traversal.at(i);
        updateGradient(traversal.at(i));
    }
}