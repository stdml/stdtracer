#pragma once
#include <chrono>
#include <string>
#include <tuple>

#include <tracer/bits/stdtracer_base.hpp>

template <typename ctx_t, typename clock_t = std::chrono::high_resolution_clock>
class scope_t_
{
    const std::string name;
    const std::chrono::time_point<clock_t> t0;
    ctx_t &ctx;

  public:
    scope_t_(const std::string &name, ctx_t &ctx)
        : name(name), t0(clock_t::now()), ctx(ctx)
    {
        ctx.in(name);
    }

    ~scope_t_() { ctx.out(name, since<double, clock_t>(t0)); }
};

template <typename ctx_t, typename clock_t = std::chrono::high_resolution_clock>
class site_scope_t_
{
    const std::chrono::time_point<clock_t> t0;
    ctx_t &ctx;

  public:
    site_scope_t_(ctx_t &ctx) : t0(clock_t::now()), ctx(ctx) { ctx.in(); }

    ~site_scope_t_() { ctx.out(since<double, clock_t>(t0)); }
};

template <typename clock_t, typename... ctx_t>
class multi_ctx_scope_t_
{
    const std::string name;
    const std::chrono::time_point<clock_t> t0;

    std::tuple<ctx_t &...> _ctxs;

  public:
    multi_ctx_scope_t_(const std::string &name, ctx_t &...ctxs)
        : name(name), t0(clock_t::now()), _ctxs({ctxs...})
    {
        all_in(_ctxs, name);
    }

    ~multi_ctx_scope_t_() { all_out(_ctxs, name, since<double, clock_t>(t0)); }
};

template <typename log_ctx_t>
class set_trace_log_t
{
  public:
    set_trace_log_t(const std::string &name, log_ctx_t &ctx, bool reuse = false)
        : name(name), ctx(ctx)
    {
        FILE *fp = reuse  //
                       ? std::fopen(name.c_str(), "a")
                       : std::fopen(name.c_str(), "w");
        ctx.log_files.push_front(fp);
        ctx.indent();
        ctx.logf1(stdout, "start logging to %s", name.c_str());
    }

    ~set_trace_log_t()
    {
        ctx.indent();
        ctx.logf1(stdout, "stop logging to file://%s", name.c_str());
        FILE *fp = ctx.log_files.front();
        ctx.log_files.pop_front();
        std::fclose(fp);
    }

  private:
    const std::string name;
    log_ctx_t &ctx;
};
