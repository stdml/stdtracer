#pragma once
#include <tracer/bits/config.hpp>
#include <tracer/bits/stdtracer_ctx_stack.hpp>
#include <tracer/bits/stdtracer_macros.hpp>
#include <tracer/bits/stdtracer_scope.hpp>

using stack_tracer_ctx_t =
    stack_tracer_ctx_t_<default_clock_t, default_duration_t>;

using stack_tracer_t = scope_t_<stack_tracer_ctx_t>;

using tracer_t = stack_tracer_t;

extern stack_tracer_ctx_t default_stack_ctx;

#define TRACE_SCOPE(name) _TRACE_SCOPE(name, tracer_t, default_stack_ctx)

#define TRACE_STMT(e) _TRACE_STMT(e, tracer_t, default_stack_ctx)

#define TRACE_EXPR(e) _TRACE_EXPR(e, tracer_t, default_stack_ctx)
