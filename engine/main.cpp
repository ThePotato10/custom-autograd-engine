#include <iostream>

#include "owner.hpp"
#include "value.hpp"
#include "traversal.hpp"

using namespace std;

int main() {
    Owner store = Owner();

    Value& a1 = store.create(0.7,   "a1");
    Value& w1 = store.create(0.55,  "w1");
    Value& b1 = store.create(-0.32, "b1");

    Value& a2 = store.create(0.3,  "a2");
    Value& w2 = store.create(-0.4, "w2");
    Value& b2 = store.create(0.82, "b2");

    Value& z = (((w1 * a1) + b1) + ((w2 * a2) + b2)).relu();

    cout << z.name << " Value: " << z.value << endl;

    z.backprop();

    return 0;
}