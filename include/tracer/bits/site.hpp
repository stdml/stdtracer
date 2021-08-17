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
