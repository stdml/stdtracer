#pragma once

// trace contexts
#include <bits/stdtracer_log_ctx.hpp>
#include <bits/stdtracer_simple_ctx.hpp>
#include <bits/stdtracer_stack_ctx.hpp>

// trace scopes
#include <bits/stdtracer_scope.hpp>

using default_clock_t = std::chrono::high_resolution_clock;
using default_duration_t = std::chrono::duration<double>;

using stack_tracer_ctx_t =
    stack_tracer_ctx_t_<default_clock_t, default_duration_t>;

using log_tracer_ctx_t = log_tracer_ctx_t_<default_clock_t, default_duration_t>;

using simple_tracer_ctx_t =
    simple_tracer_ctx_t_<default_clock_t, default_duration_t>;

using simple_tracer_t = scope_t_<simple_tracer_ctx_t>;

using multi_tracer_t =
    multi_ctx_scope_t_<default_clock_t, simple_tracer_ctx_t, log_tracer_ctx_t>;

extern simple_tracer_ctx_t default_simple_ctx;
extern log_tracer_ctx_t default_log_ctx;

using default_tracer_t = multi_tracer_t;

template <bool enable = false, typename F, typename... Arg>
void trace_call(const std::string &name, F &f, Arg &... args)
{
    if (enable) {
        default_tracer_t _(name, default_simple_ctx, default_log_ctx);
        f(args...);
    } else {
        f(args...);
    }
}

template <typename... Args> void logf(const Args &... args)
{
    default_log_ctx.logf(args...);
}
