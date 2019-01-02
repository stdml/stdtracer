#pragma once
#include <stdtracer.hpp>

using stat_tracer_ctx_t =
    stat_tracer_ctx_t_<default_clock_t, default_duration_t>;

using tracer_t = multi_ctx_scope_t_<default_clock_t, simple_tracer_ctx_t,
                                    log_tracer_ctx_t, stat_tracer_ctx_t>;

extern simple_tracer_ctx_t default_simple_ctx;
extern log_tracer_ctx_t default_log_ctx;
extern stat_tracer_ctx_t default_stat_ctx;

#define TRACE_SCOPE(name)                                                      \
    tracer_t _((name), default_simple_ctx, default_log_ctx, default_stat_ctx)

#define TRACE_STMT(e)                                                          \
    {                                                                          \
        tracer_t _(#e, default_simple_ctx, default_log_ctx, default_stat_ctx); \
        e;                                                                     \
    }

#define TRACE_EXPR(e)                                                          \
    [&]() {                                                                    \
        tracer_t _(#e, default_simple_ctx, default_log_ctx, default_stat_ctx); \
        return (e);                                                            \
    }()
