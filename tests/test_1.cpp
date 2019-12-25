#include "testing.hpp"

#include <stdtracer>

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
