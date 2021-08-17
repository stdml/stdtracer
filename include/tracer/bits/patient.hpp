#pragma once
#include <chrono>
#include <cstdint>
#include <string>

// #include <stdml/bits/collective/stat.hpp>  // for rchan::show_duration

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

  public:
    patient_t_(int n, std::string name = "")
        : patient_t_(std::chrono::seconds(n), name)
    {
    }

    patient_t_(Duration threshold = std::chrono::seconds(1),
               std::string name = "")
        : name_(std::move(name)), t0_(Clock::now()), threshold_(threshold),
          last_(t0_), always_ok_(true)
    {
    }

    ~patient_t_()
    {
        Duration d = Clock::now() - t0_;
        if (!always_ok_) {
            fprintf(stderr, "eventually finished in %.2fs | %s\n", d.count(),
                    name_.c_str());
        }
    }

    // std::string show() const { return rchan::show_duration(threshold_); }

    void report(std::string msg) const
    {
        Duration d = Clock::now() - t0_;
        fprintf(stderr, "still running %s, took %.2f | %s\n", msg.c_str(),
                d.count(), name_.c_str());
    }

    void reset()
    {
        always_ok_ &= ok();
        last_ = Clock::now();
    }

    bool ok() const { return (Clock::now() - last_) < threshold_; }
};
