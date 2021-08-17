#pragma once
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <string>

#include <tracer/bits/config.hpp>
#include <tracer/bits/stdtracer_scope.hpp>

class site_reporter_
{
    const std::string filename_;
    const std::string name_;
    const int lino_;

    FILE *fp_;
    bool report_;

  public:
    site_reporter_(std::string filename, int lino, std::string name)
        : filename_(std::move(filename)), lino_(lino), name_(std::move(name)),
          fp_(stderr), report_(report_stdout())
    {
    }

    template <typename D, typename N>
    void operator()(D d, N n) const
    {
        if (!report_) { return; }
        fprintf(fp_,
                "%%%% site called %8" PRId32
                " times, mean: %8.3fms, total: %8.3fs, file://%s:%d : %s\n",
                n, d.count() * 1e3 / n, d.count(), filename_.c_str(), lino_,
                name_.c_str());
    }
};

template <typename clock_t, typename duration_t>
class simple_site_ctx_t_
{
    site_reporter_ report;

    using N = int32_t;

    N count_;
    duration_t duration_;

  public:
    simple_site_ctx_t_(std::string filename, int lino, std::string name)
        : report(std::move(filename), lino, std::move(name))
    {
    }

    ~simple_site_ctx_t_() { report(duration_, count_); }

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
