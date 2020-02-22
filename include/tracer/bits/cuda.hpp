#pragma once
#include <tracer/bits/config.hpp>
#include <tracer/bits/stdtracer_ctx_simple.hpp>
#include <tracer/bits/stdtracer_cuda_scope.hpp>
#include <tracer/bits/stdtracer_macros.hpp>

using simple_tracer_ctx_t =
    simple_tracer_ctx_t_<default_clock_t, default_duration_t>;

using cuda_tracer_t = basic_cuda_scope<simple_tracer_ctx_t>;

using tracer_t = cuda_tracer_t;

extern simple_tracer_ctx_t default_simple_ctx;

#define TRACE_SCOPE(name) _TRACE_SCOPE(name, tracer_t, default_simple_ctx)

#define TRACE_STMT(e) _TRACE_STMT(e, tracer_t, default_simple_ctx)

#define TRACE_EXPR(e) _TRACE_EXPR(e, tracer_t, default_simple_ctx)
