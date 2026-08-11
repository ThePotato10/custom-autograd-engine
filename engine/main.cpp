#include <iostream>

#include "owner.hpp"
#include "value.hpp"
#include "traversal.hpp"

using namespace std;

int main() {
    Owner store = Owner();

    Value& a = store.create(1.0, "a");
    Value& b = store.create(2.0, "b");

    Value& c = a + b;
    Value& d = a * b;

    c.name = "c";
    d.name = "d";

    cout << c.toString();
    cout << d.toString();

    vector<Value*> traversal = buildTraversal(d);

    cout << "Traversal: \n";

    for (size_t i = 0; i < traversal.size(); ++i) {
        cout << traversal.at(i)->name << " ";
    }
}