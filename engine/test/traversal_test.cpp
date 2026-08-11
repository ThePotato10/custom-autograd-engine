#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>

#include "../value.hpp"
#include "../owner.hpp"
#include "../traversal.hpp"

// Independently re-derives the "true" reachable set from root, WITHOUT
// using your traversal function -- so a bug in your traversal can't
// accidentally validate itself.
void collectReachable(Value* node, std::unordered_set<Value*>& seen) {
    if (seen.count(node)) return;
    seen.insert(node);
    for (Value* p : node->parents) collectReachable(p, seen);
}

bool checkNoDuplicates(const std::vector<Value*>& order, std::string& err) {
    std::unordered_set<Value*> seen;
    for (Value* v : order) {
        if (seen.count(v)) {
            err = "Node '" + v->name + "' appears more than once in the order.";
            return false;
        }
        seen.insert(v);
    }
    return true;
}

bool checkCompleteness(const std::vector<Value*>& order, Value* root, std::string& err) {
    std::unordered_set<Value*> expected;
    collectReachable(root, expected);
    std::unordered_set<Value*> actual(order.begin(), order.end());

    if (expected.size() != actual.size()) {
        err = "Expected " + std::to_string(expected.size()) + " reachable nodes, order has "
              + std::to_string(actual.size()) + ".";
        return false;
    }
    for (Value* v : expected) {
        if (!actual.count(v)) {
            err = "Node '" + v->name + "' is reachable from root but missing from the order.";
            return false;
        }
    }
    return true;
}

bool checkTopoOrder(const std::vector<Value*>& order, std::string& err) {
    std::unordered_map<Value*, int> position;
    for (size_t i = 0; i < order.size(); ++i) position[order[i]] = (int)i;

    for (size_t i = 0; i < order.size(); ++i) {
        Value* node = order[i];
        for (Value* parent : node->parents) {
            if (!position.count(parent)) {
                err = "Node '" + node->name + "' has parent '" + parent->name
                      + "' that never appears in the order at all.";
                return false;
            }
            if (position[parent] >= (int)i) {
                err = "Ordering violated: parent '" + parent->name + "' (pos "
                      + std::to_string(position[parent]) + ") does not come before child '"
                      + node->name + "' (pos " + std::to_string(i) + ").";
                return false;
            }
        }
    }
    return true;
}

void runCheck(const std::string& testName, Value* root, int& passed, int& total) {
    total++;
    std::vector<Value*> order = buildTraversal(*root); // <-- adjust name/signature if needed

    std::string err;
    bool ok = checkNoDuplicates(order, err)
           && checkCompleteness(order, root, err)
           && checkTopoOrder(order, err);

    if (ok) {
        std::cout << "[PASS] " << testName << " (order size " << order.size() << ")\n";
        passed++;
    } else {
        std::cout << "[FAIL] " << testName << ": " << err << "\n";
        std::cout << "        Order was: ";
        for (Value* v : order) std::cout << v->name << " ";
        std::cout << "\n";
    }
}

void testTraversal() {
    int passed = 0, total = 0;

    {   // Single leaf, no parents at all.
        Owner store;
        Value& a = store.create(1.0, "a");
        runCheck("Single leaf node", &a, passed, total);
        std::cout << "\n";
    }
    {   // Simple chain, no branching or reuse.
        Owner store;
        Value& a = store.create(1.0, "a");
        Value& b = store.create(2.0, "b");
        Value& c = a + b;
        runCheck("Simple chain (a + b)", &c, passed, total);
        std::cout << "\n";
    }
    {   // Classic diamond: x feeds two children that both feed the root.
        // x must appear exactly once, before both y and z.
        Owner store;
        Value& x = store.create(2.0, "x");
        Value& two = store.create(2.0, "two");
        Value& three = store.create(3.0, "three");
        Value& y = x * two;
        Value& z = x * three;
        Value& w = y + z;
        runCheck("Diamond (shared ancestor, two children)", &w, passed, total);
        std::cout << "\n";
    }
    {   // Self-use: the SAME node used twice as an operand in one op.
        // parents will contain 'a' twice; the order must still list it once.
        Owner store;
        Value& a = store.create(4.0, "a");
        Value& d = a * a;
        runCheck("Self-multiplication (a * a)", &d, passed, total);
        std::cout << "\n";
    }
    {   // Asymmetric diamond: x reused at different depths, and the
        // intermediate node reused too -- stresses visited-check at
        // multiple recursion depths.
        Owner store;
        Value& x = store.create(1.5, "x");
        Value& p1 = x + x;
        Value& p2 = p1 + x;
        Value& root = p1 + p2;
        runCheck("Asymmetric diamond (multi-depth reuse)", &root, passed, total);
        std::cout << "\n";
    }
    {   // Disjoint graph: an unrelated second graph must NOT leak into
        // the traversal of 'c' just because both live in the same store.
        Owner store;
        Value& a = store.create(1.0, "a");
        Value& b = store.create(2.0, "b");
        Value& c = a + b;
        Value& x = store.create(9.0, "x_unrelated");
        Value& y = store.create(9.0, "y_unrelated");
        Value& z = x + y;
        (void)z;
        runCheck("Disjoint graph isolation", &c, passed, total);
        std::cout << "\n";
    }
    {   // Wider graph combining branching and reuse together.
        Owner store;
        Value& a = store.create(1.0, "a");
        Value& b = store.create(2.0, "b");
        Value& c = store.create(3.0, "c");
        Value& ab = a + b;
        Value& bc = b + c;
        Value& abc = ab + bc;
        Value& root = abc + a;
        runCheck("Wider graph (branch + reuse combined)", &root, passed, total);
        std::cout << "\n";
    }

    std::cout << "\n" << passed << "/" << total << " tests passed.\n";
}

int main() {
    testTraversal();
    return 0;
}