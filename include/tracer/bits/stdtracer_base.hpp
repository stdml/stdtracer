#pragma once
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <tuple>
#include <utility>

constexpr const char *STD_TRACER_REPORT_STDOUT = "STD_TRACER_REPORT_STDOUT";

constexpr const char *header_fmt = "%5s  %8s    %16s    %12s    %12s    %s\n";
constexpr const char *row_fmt = "%5d  %8d    %16f    %12.2f    %12.4f    %s\n";

template <typename T, typename clock_t>
std::chrono::duration<T> since(const std::chrono::time_point<clock_t> &t0)
{
    return clock_t::now() - t0;
}

template <size_t i>
struct seq_in;

template <>
struct seq_in<0> {
    template <typename Tuple>
    void operator()(const Tuple &t, const std::string &name) const
    {
        std::get<std::tuple_size<Tuple>::value - 1>(t).in(name);
    }
};

template <size_t i>
struct seq_in {
    template <typename Tuple>
    void operator()(const Tuple &t, const std::string &name) const
    {
        std::get<std::tuple_size<Tuple>::value - 1 - i>(t).in(name);
        seq_in<i - 1>()(t, name);
    }
};

template <typename Tuple>
void all_in(const Tuple &t, const std::string &name)
{
    seq_in<std::tuple_size<Tuple>::value - 1>()(t, name);
}

template <size_t i>
struct seq_out;

template <>
struct seq_out<0> {
    template <typename Tuple, typename Duration>
    void operator()(const Tuple &t, const std::string &name,
                    const Duration &d) const
    {
        std::get<0>(t).out(name, d);
    }
};

template <size_t i>
struct seq_out {
    template <typename Tuple, typename Duration>
    void operator()(const Tuple &t, const std::string &name,
                    const Duration &d) const
    {
        std::get<i>(t).out(name, d);
        seq_out<i - 1>()(t, name, d);
    }
};

template <typename Tuple, typename Duration>
void all_out(const Tuple &t, const std::string &name, const Duration &d)
{
    seq_out<std::tuple_size<Tuple>::value - 1>()(t, name, d);
}

inline bool report_stdout()
{
    const char *ptr = std::getenv(STD_TRACER_REPORT_STDOUT);
    if (ptr) {
        if (std::strcmp(ptr, "0") == 0) { return false; }
    }
    return true;
}
