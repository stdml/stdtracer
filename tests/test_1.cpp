#include <stdtracer>

#include <gtest/gtest.h>

void f() { TRACE(__func__); }

void g()
{
    TRACE(__func__);
    for (int i = 0; i < 10; ++i) { f(); }
}

TEST(suite, test1)
{
    TRACE(__func__);
    g();
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
