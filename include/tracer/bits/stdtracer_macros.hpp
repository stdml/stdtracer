#pragma once

#define _TRACE_SCOPE(name, SCOPE, ...) SCOPE _((name), __VA_ARGS__)

#define _TRACE_STMT(e, SCOPE, ...)                                             \
    {                                                                          \
        SCOPE _(#e, __VA_ARGS__);                                              \
        e;                                                                     \
    }

#define _TRACE_EXPR(e, SCOPE, ...)                                             \
    [&]() {                                                                    \
        SCOPE _(#e, __VA_ARGS__);                                              \
        return (e);                                                            \
    }()
