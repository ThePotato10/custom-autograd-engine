#include <iostream>

#include "owner.hpp"
#include "value.hpp"
#include "traversal.hpp"

using namespace std;

int main() {
    Owner store = Owner();

    Value& a = store.create(2.0, "a");
    Value& b = store.create(4.0, "b");
    Value& c = (a * b).exp(2);

    cout << c.name << " Value: " << c.value;

    return 0;
}