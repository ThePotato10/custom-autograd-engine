#include <iostream>

#include "owner.hpp"
#include "value.hpp"
#include "traversal.hpp"

using namespace std;

int main() {
    Owner store = Owner();

    Value& a = store.create(2.0, "a");
    Value& b = a.exp(2);

    cout << b.name << " Value: " << b.value;

    return 0;
}