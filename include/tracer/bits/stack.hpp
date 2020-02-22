#pragma once
#include <bits/stdtracer_ctx_stack.hpp>
#include <bits/stdtracer_scope.hpp>

// BEGIN defaults
using default_clock_t = std::chrono::high_resolution_clock;
using default_duration_t = std::chrono::duration<double>;

using stack_tracer_ctx_t =
    stack_tracer_ctx_t_<default_clock_t, default_duration_t>;
// END defaults

using stack_tracer_t = scope_t_<stack_tracer_ctx_t>;

using tracer_t = stack_tracer_t;

extern stack_tracer_ctx_t default_stack_ctx;

#define TRACE_SCOPE(name) tracer_t _((name), default_stack_ctx)

#define TRACE_STMT(e)                                                          \
    {                                                                          \
        tracer_t _(#e, default_stack_ctx);                                     \
        e;                                                                     \
    }

#define TRACE_EXPR(e)                                                          \
    [&]() {                                                                    \
        tracer_t _(#e, default_stack_ctx);                                     \
        return (e);                                                            \
    }()
