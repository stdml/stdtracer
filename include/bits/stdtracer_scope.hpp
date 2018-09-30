#pragma once
#include <chrono>
#include <string>

#include "stdtracer_base.hpp"

template <typename ctx_t, typename clock_t = std::chrono::high_resolution_clock>
class scope_t_
{
  public:
    scope_t_(const std::string &name, ctx_t &ctx)
        : name(name), t0(clock_t::now()), ctx(ctx)
    {
        ctx.in(name);
    }

    ~scope_t_() { ctx.out(name, since<double, clock_t>(t0)); }

  private:
    const std::string name;
    const std::chrono::time_point<clock_t> t0;
    ctx_t &ctx;
};
