#pragma once
#include <bits/stdtracer_ctx_simple.hpp>
#include <bits/stdtracer_scope.hpp>

// BEGIN defaults
using default_clock_t = std::chrono::high_resolution_clock;
using default_duration_t = std::chrono::duration<double>;

using simple_tracer_ctx_t =
    simple_tracer_ctx_t_<default_clock_t, default_duration_t>;
// END defaults

using simple_tracer_t = scope_t_<simple_tracer_ctx_t>;

using tracer_t = simple_tracer_t;

extern simple_tracer_ctx_t default_simple_ctx;

#define TRACE_SCOPE(name) tracer_t _((name), default_simple_ctx)

#define TRACE_STMT(e)                                                          \
    {                                                                          \
        tracer_t _(#e, default_simple_ctx);                                    \
        e;                                                                     \
    }

#define TRACE_EXPR(e)                                                          \
    [&]() {                                                                    \
        tracer_t _(#e, default_simple_ctx);                                    \
        return (e);                                                            \
    }()
