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
    const int lino_;
    const std::string name_;

    FILE *fp_;
    bool report_;

    static std::string remove_prefix(std::string filepath)
    {
#ifdef STD_TRACER_SRC_ROOT_DIR
        static constexpr const char *SRC_DIR = STD_TRACER_SRC_ROOT_DIR;
        static const int l = strlen(SRC_DIR);
        if (strncmp(SRC_DIR, filepath.c_str(), l) == 0) {
            filepath.erase(filepath.begin(), filepath.begin() + l);
            filepath = '.' + filepath;
        }
#endif
        return filepath;
    }

  public:
    site_reporter_(std::string filename, int lino, std::string name)
        : filename_(remove_prefix(std::move(filename))), lino_(lino),
          name_(std::move(name)), fp_(stderr), report_(report_stdout())
    {
    }

    template <typename D, typename N>
    void operator()(D d, N n) const
    {
        if (!report_) { return; }
        fprintf(fp_,
                "%%%% site called %8" PRId32
                " times, mean: %8.3fms, total: %8.3fs, %s:%d : %s\n",
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
