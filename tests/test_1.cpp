#include "testing.hpp"

#include <tracer/simple>

DEFINE_TRACE_CONTEXTS;

void f() { TRACE_SCOPE(__func__); }

void g()
{
    TRACE_SCOPE(__func__);
    for (int i = 0; i < 10; ++i) { f(); }
}

TEST(suite, test1)
{
    TRACE_SCOPE(__func__);
    g();
}
