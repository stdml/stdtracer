#pragma once
#include <tracer/bits/config.hpp>
#include <tracer/bits/stdtracer_ctx_log.hpp>
#include <tracer/bits/stdtracer_ctx_simple.hpp>
#include <tracer/bits/stdtracer_macros.hpp>
#include <tracer/bits/stdtracer_scope.hpp>

using log_tracer_ctx_t = log_tracer_ctx_t_<default_clock_t, default_duration_t>;

using simple_tracer_ctx_t =
    simple_tracer_ctx_t_<default_clock_t, default_duration_t>;

using simple_log_tracer_t =
    multi_ctx_scope_t_<default_clock_t, simple_tracer_ctx_t, log_tracer_ctx_t>;

using tracer_t = simple_log_tracer_t;

extern simple_tracer_ctx_t default_simple_ctx;
extern log_tracer_ctx_t default_log_ctx;

#define TRACE_SCOPE(name)                                                      \
    _TRACE_SCOPE(name, tracer_t, default_simple_ctx, default_log_ctx)

#define TRACE_STMT(e)                                                          \
    _TRACE_STMT(e, tracer_t, default_simple_ctx, default_log_ctx)

#define TRACE_EXPR(e)                                                          \
    _TRACE_EXPR(e, tracer_t, default_simple_ctx, default_log_ctx)
