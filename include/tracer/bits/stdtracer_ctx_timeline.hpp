#pragma once
#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include <tracer/bits/stdtracer_base.hpp>

template <typename clock_t, typename duration_t>
class timeline_tracer_ctx_t_
{
    using instant_t = std::chrono::time_point<clock_t>;

    struct event {
        instant_t t0;
        instant_t t1;
        std::string name;
    };

    using timeline = std::vector<event>;

    const std::string name;
    const instant_t t0;
    const std::string report_file;
    const bool report_stdout_;

    timeline timeline_;

  public:
    explicit timeline_tracer_ctx_t_(const std::string name,
                                    const std::string report_file = "")
        : name(std::move(name)), t0(clock_t::now()),
          report_file(std::move(report_file)), report_stdout_(report_stdout())
    {
    }

    ~timeline_tracer_ctx_t_() { report(stdout); }

    void report();

    void in() {}

    void out(const std::string name, const instant_t t0, const instant_t t1)
    {
        timeline_.emplace_back(event{t0, t1, std::move(name)});
    }

    void report(FILE *fp) const
    {
        printf("timeline(%s)\n", name.c_str());
        std::string hr(80, '-');
        printf("%s\n", hr.c_str());
        for (const auto &e : timeline_) {
            const auto t0 = e.t0.time_since_epoch();
            const auto t1 = e.t1.time_since_epoch();
            printf("`` %lld %lld %s\n", t0.count(), t1.count(), e.name.c_str());
        }
    }
};
