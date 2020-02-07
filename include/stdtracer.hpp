#pragma once

// trace contexts
#include <bits/stdtracer_ctx_log.hpp>
#include <bits/stdtracer_ctx_simple.hpp>
#include <bits/stdtracer_ctx_stack.hpp>
#include <bits/stdtracer_ctx_thread.hpp>

// trace scopes
#include <bits/stdtracer_scope.hpp>

using default_clock_t = std::chrono::high_resolution_clock;
using default_duration_t = std::chrono::duration<double>;

// builtin contexts
using stack_tracer_ctx_t =
    stack_tracer_ctx_t_<default_clock_t, default_duration_t>;

using log_tracer_ctx_t = log_tracer_ctx_t_<default_clock_t, default_duration_t>;

using simple_tracer_ctx_t =
    simple_tracer_ctx_t_<default_clock_t, default_duration_t>;

using thread_tracer_ctx_t =
    thread_tracer_ctx_t_<default_clock_t, default_duration_t>;

// builtin scopes
using simple_tracer_t = scope_t_<simple_tracer_ctx_t>;

using simple_log_tracer_t =
    multi_ctx_scope_t_<default_clock_t, simple_tracer_ctx_t, log_tracer_ctx_t>;
