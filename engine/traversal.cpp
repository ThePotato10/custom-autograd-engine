#include <vector>
#include <algorithm>

#include "value.hpp"

using std::vector;

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

void visit(Value* entry, vector<Value*>& visited, vector<Value*>& output) {
    if (std::find(visited.begin(), visited.end(), entry) != visited.end()) return; // If the current Value has already been visited, then we end the recursion here

    visited.push_back(entry); // Mark Value as visited
    for (size_t i = 0; i < entry->parents.size(); ++i) {
        visit(entry->parents.at(i), visited, output); // recurse into Value's parents
    }

    output.push_back(entry); // Append value to traversal after clearing all its parents first recursively
}

vector<Value*> buildTraversal(Value& entry) {
    // These need to be passed by reference to the visit function so that they are consistent across recursion branches
    vector<Value*> traversal = vector<Value*>{}; 
    vector<Value*> visited   = vector<Value*>{};

    visit(&entry, visited, traversal);

    return traversal;
}