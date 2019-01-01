#include "benchmark.hpp"

template <int k> struct bench_simple {
    void operator()(benchmark::State &state) const
    {
        using ctx_t =
            simple_tracer_ctx_t_<default_clock_t, default_duration_t, true>;
        using tracer_t = scope_t_<ctx_t>;

        ctx_t local("local");

        int counter = 0;
        for (auto _ : state) {
            tracer_t __(std::to_string(counter % k), local);
            ++counter;
        }
    }
};

static void bench_simple_10(benchmark::State &state)
{
    bench_simple<10>()(state);
}
BENCHMARK(bench_simple_10);

static void bench_simple_100(benchmark::State &state)
{
    bench_simple<100>()(state);
}
BENCHMARK(bench_simple_100);

template <int k> struct bench_stack {
    void operator()(benchmark::State &state) const
    {
        using ctx_t =
            stack_tracer_ctx_t_<default_clock_t, default_duration_t, true>;
        using tracer_t = scope_t_<ctx_t>;

        ctx_t local("local");

        int counter = 0;
        for (auto _ : state) {
            tracer_t __(std::to_string(counter % 10), local);
            ++counter;
        }
    }
};

static void bench_stack_10(benchmark::State &state)
{
    bench_stack<10>()(state);
}
BENCHMARK(bench_stack_10);

static void bench_stack_100(benchmark::State &state)
{
    bench_stack<100>()(state);
}
BENCHMARK(bench_stack_100);

template <int k> struct bench_multi {
    void operator()(benchmark::State &state) const
    {
        using ctx_t_1 =
            simple_tracer_ctx_t_<default_clock_t, default_duration_t, true>;
        using ctx_t_2 =
            stack_tracer_ctx_t_<default_clock_t, default_duration_t, true>;

        using tracer_t = multi_ctx_scope_t_<default_clock_t, ctx_t_1, ctx_t_2>;

        ctx_t_1 c1("local");
        ctx_t_2 c2("local");

        int counter = 0;
        for (auto _ : state) {
            tracer_t __(std::to_string(counter % 10), c1, c2);
            ++counter;
        }
    }
};

static void bench_multi_10(benchmark::State &state)
{
    bench_multi<10>()(state);
}
BENCHMARK(bench_multi_10);

static void bench_multi_100(benchmark::State &state)
{
    bench_multi<100>()(state);
}
BENCHMARK(bench_multi_100);

template <int k> struct bench_thread {
    void operator()(benchmark::State &state) const
    {
        using ctx_t =
            thread_tracer_ctx_t_<default_clock_t, default_duration_t, true>;
        using tracer_t = scope_t_<ctx_t>;

        ctx_t local("local");

        int counter = 0;
        for (auto _ : state) {
            tracer_t __(std::to_string(counter % 10), local);
            ++counter;
        }
    }
};

static void bench_thread_10(benchmark::State &state)
{
    bench_thread<10>()(state);
}
BENCHMARK(bench_thread_10);

static void bench_thread_100(benchmark::State &state)
{
    bench_thread<100>()(state);
}
BENCHMARK(bench_thread_100);
