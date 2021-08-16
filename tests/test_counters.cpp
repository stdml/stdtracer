#include "testing.hpp"
#include <thread>

#include <tracer/bits/counters.hpp>

void f()
{
    using ave_t = basic_counter<float, mean_accumulator<float>>;
    using counts_t = basic_counter<uint32_t, count_accumulator<uint32_t>>;
    using sum_t = basic_counter<int, sum_accumulator<int>>;
    using min_t = basic_counter<int, min_accumulator<int>>;
    using max_t = basic_counter<int, max_accumulator<int>>;

    counts_t c;
    ave_t r;
    sum_t s;
    min_t l;
    max_t u;

    for (int i = 0; i < 10; ++i) {
        c(i);
        r(i);
        s(i);
        l(i);
        u(i);
    }

    ASSERT_EQ(static_cast<int>(c), 10);
    ASSERT_EQ(static_cast<float>(r), static_cast<float>(4.5));
    ASSERT_EQ(static_cast<int>(s), 45);
    ASSERT_EQ(static_cast<int>(l), 0);
    ASSERT_EQ(static_cast<int>(u), 9);
}

TEST(suite, test1) { f(); }
