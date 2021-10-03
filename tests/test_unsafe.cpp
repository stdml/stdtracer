#include "testing.hpp"

#include <tracer/unsafe>

void f1() { throw std::runtime_error(""); }
void g1() { throw std::runtime_error(""); }

TEST(suite, test_1)
{
    int caught = 0;
    try {
        UNSAFE_EXPR(g1());
    } catch (...) {
        ++caught;
    }

    try {
        UNSAFE_EXPR_HINT(f1(), "call f");
    } catch (...) {
        ++caught;
    }

    ASSERT_EQ(caught, 2);
}
