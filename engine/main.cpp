#include <iostream>

#include "owner.hpp"
#include "value.hpp"

using namespace std;

int main() {
    Owner store = Owner();

    Value& a = store.create(1.0, "a");
    Value& b = store.create(2.0, "b");

    Value& c = a + b;
    Value& d = a * b;

    cout << c.toString();
    cout << d.toString();
}