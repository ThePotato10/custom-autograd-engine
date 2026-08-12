#include <iostream>
#include <string>
#include <cmath>

#include "../owner.hpp"
#include "../value.hpp"

// Approximate equality for doubles -- never compare floating point with ==,
// since even mathematically "exact" results (e.g. 0.1 + 0.2) can differ from
// the expected value in the last bit or two due to rounding.
bool approxEqual(double a, double b, double epsilon = 1e-6) {
    return std::fabs(a - b) < epsilon;
}

bool checkGradient(const std::string& label, Value& v, double expected, std::string& err) {
    if (!approxEqual(v.gradient, expected)) {
        err = "Expected " + label + ".gradient == " + std::to_string(expected)
              + ", got " + std::to_string(v.gradient);
        return false;
    }
    return true;
}

void runCheck(const std::string& testName, bool ok, const std::string& err,
              int& passed, int& total) {
    total++;
    if (ok) {
        std::cout << "[PASS] " << testName << "\n";
        passed++;
    } else {
        std::cout << "[FAIL] " << testName << ": " << err << "\n";
    }
}

void testBackward() {
    int passed = 0, total = 0;
    std::string err;

    {   // Baseline: single op, distinct operands. This is the case you
        // already verified by hand -- c = a*b.
        Owner store;
        Value& a = store.create(2.0, "a");
        Value& b = store.create(3.0, "b");
        Value& c = a * b;
        c.backprop();

        bool ok = checkGradient("c", c, 1.0, err)
               && checkGradient("a", a, 3.0, err)
               && checkGradient("b", b, 2.0, err);
        runCheck("MUL: c = a*b", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // ADD: both parents get gradient 1, regardless of value.
        Owner store;
        Value& a = store.create(5.0, "a");
        Value& b = store.create(-2.0, "b");
        Value& c = a + b;
        c.backprop();

        bool ok = checkGradient("a", a, 1.0, err) && checkGradient("b", b, 1.0, err);
        runCheck("ADD: c = a+b", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // SUB: gradient sign must differ between operands (dc/da=1, dc/db=-1).
        // This specifically catches a backward rule that treats SUB like ADD
        // and forgets to negate the second operand's contribution.
        Owner store;
        Value& a = store.create(5.0, "a");
        Value& b = store.create(2.0, "b");
        Value& c = a - b;
        c.backprop();

        bool ok = checkGradient("a", a, 1.0, err) && checkGradient("b", b, -1.0, err);
        runCheck("SUB: c = a-b (asymmetric gradient sign)", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // DIV: dc/da = 1/b, dc/db = -a/b^2. Catches a backward rule that
        // reuses the MUL rule (swap operand values) instead of the actual
        // quotient rule.
        Owner store;
        Value& a = store.create(6.0, "a");
        Value& b = store.create(3.0, "b");
        Value& c = a / b;
        c.backprop();

        bool ok = checkGradient("a", a, 1.0 / 3.0, err)
               && checkGradient("b", b, -6.0 / 9.0, err);
        runCheck("DIV: c = a/b", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // POWER: c = a^3, dc/da = 3*a^2. Catches an off-by-one in the
        // exponent rule (e.g. forgetting to subtract 1, or using a^n
        // instead of n*a^(n-1)).
        Owner store;
        Value& a = store.create(2.0, "a");
        Value& c = a.exp(3.0);           
        c.backprop();

        bool ok = checkGradient("a", a, 3.0 * std::pow(2.0, 2.0), err);
        runCheck("POWER: c = a^3", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // Straight-line chain: gradient must actually propagate through an
        // intermediate node, not just from root's direct parents. Catches a
        // backward pass that stops after one layer instead of continuing to
        // reversed-topo-order completion.
        // d = (a + b) * c
        Owner store;
        Value& a = store.create(2.0, "a");
        Value& b = store.create(3.0, "b");
        Value& c = store.create(4.0, "c");
        Value& sum = a + b;      // sum = 5
        Value& d = sum * c;      // d = 20
        d.backprop();

        // dd/dc = sum = 5
        // dd/dsum = c = 4  -->  dd/da = 4, dd/db = 4  (chain rule through sum)
        bool ok = checkGradient("c", c, 5.0, err)
               && checkGradient("a", a, 4.0, err)
               && checkGradient("b", b, 4.0, err);
        runCheck("Chain: d = (a+b)*c", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // Diamond / accumulation: x feeds two children that both feed root.
        // x's gradient must be the SUM of both paths' contributions -- this
        // is the single most important case to test, since it's exactly
        // what breaks if backward() assigns (=) instead of accumulates (+=).
        // y = x*2, z = x*3, w = y+z  -->  dw/dx = 2 + 3 = 5
        Owner store;
        Value& x = store.create(4.0, "x");
        Value& two = store.create(2.0, "two");
        Value& three = store.create(3.0, "three");
        Value& y = x * two;
        Value& z = x * three;
        Value& w = y + z;
        w.backprop();

        bool ok = checkGradient("x", x, 5.0, err);
        runCheck("Diamond accumulation: w = x*2 + x*3", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // Self-use within a single op: d = a*a. Product rule gives
        // dd/da = a + a = 2a, NOT just "a" (which is what you'd wrongly get
        // if backward() only pushes gradient to ONE of the two identical
        // parent pointers instead of both entries in parents).
        Owner store;
        Value& a = store.create(5.0, "a");
        Value& d = a * a;
        d.backprop();

        bool ok = checkGradient("a", a, 10.0, err);
        runCheck("Self-multiplication: d = a*a", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // Asymmetric diamond (same shape as your traversal-order bug case):
        // p1 = x+x, p2 = p1+x, root = p1+p2.
        // By hand: root = p1 + p2 = p1 + (p1+x) = 2*p1 + x = 2*(2x) + x = 5x
        // droot/dx = 5. This is the case most likely to silently produce a
        // wrong (but plausible-looking) number if traversal order is subtly
        // off, since it combines branching AND reuse AND multiple depths.
        Owner store;
        Value& x = store.create(1.5, "x");
        Value& p1 = x + x;
        Value& p2 = p1 + x;
        Value& root = p1 + p2;
        root.backprop();

        bool ok = checkGradient("x", x, 5.0, err);
        runCheck("Asymmetric diamond: root = p1 + (p1+x), p1 = x+x", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // Isolation: a node NOT reachable from root must be untouched --
        // still its default-initialized gradient. Catches a backward() that
        // (incorrectly) walks the whole Store instead of following parents
        // from root.
        Owner store;
        Value& a = store.create(1.0, "a");
        Value& b = store.create(2.0, "b");
        Value& c = a + b;

        Value& x = store.create(9.0, "x_unrelated");
        Value& y = store.create(9.0, "y_unrelated");
        Value& unrelated = x * y;
        (void)unrelated;

        c.backprop();

        bool ok = checkGradient("x_unrelated", x, 0.0, err)
               && checkGradient("y_unrelated", y, 0.0, err);
        runCheck("Isolation: unrelated nodes stay at gradient 0", ok, err, passed, total);
        std::cout << std::endl;
    }
    {   // Root's own gradient must be seeded to 1 regardless of graph shape --
        // trivial but worth asserting explicitly, since every other test's
        // correctness assumes this seed is right.
        Owner store;
        Value& a = store.create(1.0, "a");
        Value& b = store.create(1.0, "b");
        Value& c = a + b;
        c.backprop();

        bool ok = checkGradient("c", c, 1.0, err);
        runCheck("Seed: root.gradient == 1 after backward()", ok, err, passed, total);
        std::cout << std::endl;
    }

    std::cout << "\n" << passed << "/" << total << " tests passed.\n";
}

int main() {
    testBackward();
    return 0;
}