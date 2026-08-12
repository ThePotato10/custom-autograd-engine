#include <iostream>

#include "owner.hpp"
#include "value.hpp"
#include "traversal.hpp"

using namespace std;

int main() {
    Owner store = Owner();

    Value& a = store.create(0.5, "a(L-1)");
    Value& w = store.create(0.4, "w(L)");
    Value& b = store.create(0.2, "b(L)");
    Value& y = store.create(0.6, "y");

    Value& z = (w * a) + b;
    Value& c = z - y;

    z.name = "z";
    c.name = "c";

    c.backprop();

    return 0;
}