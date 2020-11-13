#pragma once
#include <tracer/bits/config.hpp>
#include <tracer/bits/stdtracer_ctx_timeline.hpp>
#include <tracer/bits/stdtracer_macros.hpp>
#include <tracer/bits/stdtracer_scope.hpp>

using timeline_tracer_ctx_t =
    timeline_tracer_ctx_t_<default_clock_t, default_duration_t>;

using tracer_t = timeline_scope_t_<timeline_tracer_ctx_t>;

extern timeline_tracer_ctx_t default_timeline_tracer_ctx;

#define TRACE_SCOPE(name)                                                      \
    _TRACE_SCOPE(name, tracer_t, default_timeline_tracer_ctx)

#define TRACE_STMT(e) _TRACE_STMT(e, tracer_t, default_timeline_tracer_ctx)

#define TRACE_EXPR(e) _TRACE_EXPR(e, tracer_t, default_timeline_tracer_ctx)
