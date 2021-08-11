#pragma once
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <string>

#include <tracer/bits/config.hpp>
#include <tracer/bits/stdtracer_scope.hpp>

template <typename clock_t, typename duration_t>
class simple_site_ctx_t_
{
    const std::string filename_;
    const std::string name_;
    const int lino_;

    using N = int32_t;

    N count_;
    duration_t duration_;

  public:
    simple_site_ctx_t_(std::string filename, int lino, std::string name)
        : filename_(std::move(filename)), lino_(lino), name_(std::move(name))
    {
    }

    ~simple_site_ctx_t_()
    {
        fprintf(stderr,
                "%%%% site called %8" PRId32
                " times, mean: %8.3fms, total: %8.3fs, file://%s:%d : %s\n",
                count_, duration_.count() * 1e3 / count_, duration_.count(),
                filename_.c_str(), lino_, name_.c_str());
    }

    void in()
    {
        // fprintf(stderr, "entering %s\n", name_.c_str());
    }

    void out(duration_t d)
    {
        ++count_;
        duration_ += d;
    }
};

using simple_site_ctx_t =
    simple_site_ctx_t_<default_clock_t, default_duration_t>;

using site_tracer_t = site_scope_t_<simple_site_ctx_t>;

#define __DEF_SITE_SCOPE(name, f, l)                                           \
    static simple_site_ctx_t ___ctx(f, l, name);                               \
    site_tracer_t __site(___ctx);

#define _TRACE_SITE_SCOPE(name, f, l) __DEF_SITE_SCOPE(name, f, l)

#define _TRACE_SITE_STMT(e, f, l)                                              \
    {                                                                          \
        __DEF_SITE_SCOPE(#e, f, l)                                             \
        e;                                                                     \
    }

#define _TRACE_SITE_EXPR(e, f, l)                                              \
    [&] {                                                                      \
        __DEF_SITE_SCOPE(#e, f, l)                                             \
        return (e);                                                            \
    }()

#define TRACE_SITE_SCOPE(name) _TRACE_SITE_SCOPE(name, __FILE__, __LINE__)
#define TRACE_SITE_STMT(e) _TRACE_SITE_STMT(e, __FILE__, __LINE__)
#define TRACE_SITE_EXPR(e) _TRACE_SITE_EXPR(e, __FILE__, __LINE__)
