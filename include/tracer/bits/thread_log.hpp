#pragma once
#include <tracer/bits/stdtracer_ctx_thread_log.hpp>
#include <tracer/bits/stdtracer_scope.hpp>

// BEGIN defaults
using default_clock_t = std::chrono::high_resolution_clock;
using default_duration_t = std::chrono::duration<double>;

using thread_log_tracer_ctx_t =
    thread_log_tracer_ctx_t_<default_clock_t, default_duration_t>;
// END defaults

using tracer_t = scope_t_<thread_log_tracer_ctx_t>;

extern thread_log_tracer_ctx_t default_thread_log_tracer_ctx;

#define TRACE_SCOPE(name) tracer_t _((name), default_thread_log_tracer_ctx)

#define TRACE_STMT(e)                                                          \
    {                                                                          \
        tracer_t _(#e, default_thread_log_tracer_ctx);                         \
        e;                                                                     \
    }

#define TRACE_EXPR(e)                                                          \
    [&]() {                                                                    \
        tracer_t _(#e, default_thread_log_tracer_ctx);                         \
        return (e);                                                            \
    }()
