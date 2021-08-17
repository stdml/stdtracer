#pragma once
#include <chrono>
#include <cstdint>
#include <string>

#include <tracer/bits/stdtracer_base.hpp>
#include <tracer/bits/stdtracer_xterm.hpp>

class patient_t_
{
    using Clock = std::chrono::high_resolution_clock;
    using Instant = std::chrono::time_point<Clock>;
    using Duration = std::chrono::duration<double>;

    const std::string name_;
    const Instant t0_;
    const Duration threshold_;
    Instant last_;
    bool always_ok_;

    FILE *fp_;
    bool report_;

  public:
    static constexpr const char *prefix = "[patient] ";

    patient_t_(int n, std::string name = "")
        : patient_t_(std::chrono::seconds(n), name)
    {
    }

    patient_t_(Duration threshold = std::chrono::seconds(1),
               std::string name = "", FILE *fp = stderr)
        : name_(std::move(name)), t0_(Clock::now()), threshold_(threshold),
          last_(t0_), always_ok_(true), fp_(fp), report_(report_stdout())
    {
    }

    ~patient_t_()
    {
        if (!report_) { return; }
        Duration d = Clock::now() - t0_;
        if (!always_ok_) {
            fprintf(fp_, "%s%s in %.2fs | %s\n", prefix,
                    xt_green("eventually finished"), d.count(), name_.c_str());
        }
    }

    void report(std::string msg) const
    {
        if (!report_) { return; }
        Duration d = Clock::now() - t0_;
        fprintf(fp_, "%s%s %s, took %.2f | %s\n", prefix,
                xt_yellow("still running"), msg.c_str(), d.count(),
                name_.c_str());
    }

    void reset()
    {
        always_ok_ &= ok();
        last_ = Clock::now();
    }

    bool ok() const { return (Clock::now() - last_) < threshold_; }
};
