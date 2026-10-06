#include <vector>
#include <algorithm>
#include <unordered_set>

#include "value.hpp"

using std::vector;
using std::unordered_set;

/*
 * Suppose x = b + a, b = a + 3
 * buildTraversal(x);
 * x has not been visited, mark as visited and recurse into parents
 *   - visit b, mark as visited. b has a as a parent, recurse into parents. 
 *     - visit a. a has no parents, so branch ends here
 *     - visit 3. 3 has no parents, so branch ends here
 *   - visit a, already been visited, so skip,
 * x's parents have now been fully visited, so the traversal is complete. 
 */

void visit(Value* entry, unordered_set<Value*>& visited, vector<Value*>& output) {
    // If the current Value has already been visited, then we end the recursion here
    if (visited.find(entry) != visited.end()) return; 

    visited.insert(entry); // Mark Value as visited
    for (size_t i = 0; i < entry->parents.size(); ++i) {
        visit(entry->parents.at(i), visited, output); // recurse into Value's parents
    }

    output.push_back(entry); // Append value to traversal after clearing all its parents first recursively
}

vector<Value*> buildTraversal(Value& entry) {
    // These need to be passed by reference to the visit function so that they are consistent across recursion branches
    vector<Value*> traversal;

    // Using an unordered_set allows for O(1) lookups instead of O(n)
    // By using visited.find()
    // Which reduces the runtime of this algorithm from O(n^2) to O(n)
    unordered_set<Value*> visited; 

    visit(&entry, visited, traversal);

    return traversal;
}