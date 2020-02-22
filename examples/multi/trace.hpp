#pragma once
#include <bits/stdtracer_ctx_log.hpp>
#include <bits/stdtracer_ctx_simple.hpp>
#include <bits/stdtracer_scope.hpp>

using default_clock_t = std::chrono::high_resolution_clock;
using default_duration_t = std::chrono::duration<double>;

using simple_tracer_ctx_t =
    simple_tracer_ctx_t_<default_clock_t, default_duration_t>;

using log_tracer_ctx_t = log_tracer_ctx_t_<default_clock_t, default_duration_t>;

using simple_log_tracer_t =
    multi_ctx_scope_t_<default_clock_t, simple_tracer_ctx_t, log_tracer_ctx_t>;

using tracer_t = simple_log_tracer_t;

extern simple_tracer_ctx_t default_simple_ctx;
extern log_tracer_ctx_t default_log_ctx;

#define TRACE_SCOPE(name)                                                      \
    tracer_t _((name), default_simple_ctx, default_log_ctx)

#define TRACE_STMT(e)                                                          \
    {                                                                          \
        tracer_t _(#e, default_simple_ctx, default_log_ctx);                   \
        e;                                                                     \
    }

#define TRACE_EXPR(e)                                                          \
    [&]() {                                                                    \
        tracer_t _(#e, default_simple_ctx, default_log_ctx);                   \
        return (e);                                                            \
    }()
