#include "testing.hpp"

#include <tracer/patient>

int f()
{
    WITH_PATIENT(__func__);
    return 1;
}

int g()
{
    WITH_PATIENT(__func__, 1);
    return 1;
}

TEST(suite, test_1) { f(); }
