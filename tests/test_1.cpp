#include <stdtracer>

#include <gtest/gtest.h>

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

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
