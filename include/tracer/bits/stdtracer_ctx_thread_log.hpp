#pragma once
#include <algorithm>
#include <chrono>
#include <cstring>
#include <mutex>
#include <sstream>
#include <stack>
#include <string>

#include <tracer/bits/stdtracer_base.hpp>
#include <tracer/bits/stdtracer_xterm.hpp>

template <typename clock_t, typename duration_t>
class thread_log_tracer_ctx_t_
{
    const std::string name;
    const std::chrono::time_point<clock_t> t0;

    std::mutex mu;

  public:
    explicit thread_log_tracer_ctx_t_(const std::string &name)
        : name(name), t0(clock_t::now())
    {
    }

    void in(const std::string &name)
    {
        std::lock_guard<std::mutex> lk(mu);
        printf("%s |->\n", name.c_str());
    }

    void out(const std::string &name, const duration_t &d)
    {
        std::lock_guard<std::mutex> lk(mu);
        printf("%s <-|\n", name.c_str());
    }
};
