#pragma once
#include <tracer/bits/config.hpp>
#include <tracer/bits/stdtracer_ctx_simple.hpp>
#include <tracer/bits/stdtracer_macros.hpp>
#include <tracer/bits/stdtracer_scope.hpp>

using simple_tracer_ctx_t =
    simple_tracer_ctx_t_<default_clock_t, default_duration_t>;

using simple_tracer_t = scope_t_<simple_tracer_ctx_t>;

using tracer_t = simple_tracer_t;

extern simple_tracer_ctx_t default_simple_ctx;

#define TRACE_SCOPE(name) _TRACE_SCOPE(name, tracer_t, default_simple_ctx)

#define TRACE_SCOPE_(fmt, ...)                                                 \
    static char __scope_name[256];                                             \
    sprintf_s(__scope_name, 256 - 1, fmt, __VA_ARGS__);                        \
    _TRACE_SCOPE(__scope_name, tracer_t, default_simple_ctx)

#define TRACE_STMT(e) _TRACE_STMT(e, tracer_t, default_simple_ctx)

#define TRACE_EXPR(e) _TRACE_EXPR(e, tracer_t, default_simple_ctx)
