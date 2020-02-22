#pragma once
#include <tracer/bits/config.hpp>
#include <tracer/bits/stdtracer_ctx_thread_log.hpp>
#include <tracer/bits/stdtracer_macros.hpp>
#include <tracer/bits/stdtracer_scope.hpp>

using thread_log_tracer_ctx_t =
    thread_log_tracer_ctx_t_<default_clock_t, default_duration_t>;

using tracer_t = scope_t_<thread_log_tracer_ctx_t>;

extern thread_log_tracer_ctx_t default_thread_log_tracer_ctx;

#define TRACE_SCOPE(name)                                                      \
    _TRACE_SCOPE(name, tracer_t, default_thread_log_tracer_ctx)

#define TRACE_STMT(e) _TRACE_STMT(e, tracer_t, default_thread_log_tracer_ctx)

#define TRACE_EXPR(e) _TRACE_EXPR(e, tracer_t, default_thread_log_tracer_ctx)
