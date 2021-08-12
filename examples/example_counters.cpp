#include <iostream>
#include <thread>

#include <tracer/bits/counters.hpp>

int main()
{
    using C = std::chrono::high_resolution_clock;
    using D = std::chrono::duration<double>;
    using ave_t = basic_counter<float, mean_accumulator<float>>;
    using counts_t = basic_counter<uint32_t, count_accumulator<uint32_t>>;
    using mean_time_t = basic_counter<D, mean_accumulator<D>>;

    counts_t c;
    ave_t r;
    mean_time_t mt;
    for (int i = 0; i < 10; ++i) {
        auto t0 = C::now();
        using namespace std::chrono_literals;
        for (int j = 0; j < i; ++j) { std::this_thread::sleep_for(10ms); }
        c(i);
        r(i);
        mt(C::now() - t0);
    }

    std::cout << c << std::endl;
    std::cout << r << std::endl;
    std::cout << 1e3 * static_cast<D>(mt).count() << "ms" << std::endl;
    return 0;
}
