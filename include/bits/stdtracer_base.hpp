#pragma once
#include <chrono>
#include <utility>

template <typename T, typename clock_t>
std::chrono::duration<T> since(const std::chrono::time_point<clock_t> &t0)
{
    return clock_t::now() - t0;
}

#include <vector>

template <typename F> int call_in(F &f, const std::string &name)
{
    f.in(name);
    return 0;
}

template <typename F, typename Duration>
int call_out(F &f, const std::string &name, const Duration &d)
{
    f.out(name, d);
    return 0;
}

template <typename Tuple, std::size_t... I>
void all_in(const Tuple &t, const std::string &name, std::index_sequence<I...>)
{
    std::vector<int> ve({call_in(std::get<I>(t), name)...});
}

template <typename Tuple, typename Duration, std::size_t... I>
void all_out(const Tuple &t, const std::string &name, const Duration &d,
             std::index_sequence<I...>)
{
    std::vector<int> ve({call_out(
        std::get<std::tuple_size<Tuple>::value - 1 - I>(t), name, d)...});
}
