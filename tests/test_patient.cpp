#include "testing.hpp"

#include <tracer/patient>

int f()
{
    WITH_PATIENT(__func__);
    for (int i=0; i < 10; ++i) {
        YIELD_PATIENT("forloop");
    }
    return 1;
}

int g()
{
    WITH_PATIENT(__func__, 1);
    for (int i=0; i < 10; ++i) {
        YIELD_PATIENT_("forloop: %d", i);
    }
    return 1;
}

TEST(suite, test_1) { f(); }
