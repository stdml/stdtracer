#include <iostream>
#include <thread>

#include <tracer/bits/counters.hpp>

int main()
{
    using C = std::chrono::high_resolution_clock;
    using D = std::chrono::duration<double>;
    using ave_t = basic_counter<float, mean_accumulator<float>>;
    using mean_time_t = basic_counter<D, mean_accumulator<D>>;
    using counts_t = basic_counter<uint32_t, count_accumulator<uint32_t>>;

    using sum_t = basic_counter<int, sum_accumulator<int>>;
    using min_t = basic_counter<int, min_accumulator<int>>;
    using max_t = basic_counter<int, max_accumulator<int>>;

    counts_t c;
    ave_t r;
    mean_time_t mt;
    sum_t s;
    min_t l;
    max_t u;

    for (int i = 0; i < 10; ++i) {
        auto t0 = C::now();
        using namespace std::chrono_literals;
        for (int j = 0; j < i; ++j) { std::this_thread::sleep_for(10ms); }
        c(i);
        r(i);

        s(i);
        l(i);
        u(i);

        mt(C::now() - t0);
    }

    std::cout << "count: " << c << std::endl;
    std::cout << "ave: " << r << std::endl;
    std::cout << "sum: " << s << std::endl;
    std::cout << "min: " << l << std::endl;
    std::cout << "max: " << u << std::endl;

    std::cout << 1e3 * static_cast<D>(mt).count() << "ms" << std::endl;
    return 0;
}
