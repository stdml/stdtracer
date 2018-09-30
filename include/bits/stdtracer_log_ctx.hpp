#pragma once
#include <algorithm>
#include <chrono>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

#include <unistd.h>

#include "stdtracer_base.hpp"

struct xterm_t {
    const bool is_tty;
    xterm_t(uint8_t b, uint8_t f) : is_tty(isatty(fileno(stdout)))
    {
        if (is_tty) { printf("\e[%u;%um", b, f); }
    }

    ~xterm_t()
    {
        if (is_tty) { printf("\e[m"); }
    }
};

#define WITH_XTERM(b, f, e)                                                    \
    {                                                                          \
        xterm_t _(b, f);                                                       \
        e;                                                                     \
    }

template <typename clock_t, typename duration_t> class log_tracer_ctx_t_
{
  public:
    explicit log_tracer_ctx_t_(const std::string &name)
        : name(name), t0(clock_t::now()), depth(0)
    {
        log_files.push_front(stdout);
    }

    ~log_tracer_ctx_t_()
    {
        constexpr const char *filename = "trace.log";
        logf("profile info logged to %s", filename);
        FILE *fp = fopen(filename, "w");
        report(fp);
        fclose(fp);
        report(stdout);
    }

    void in(const std::string &name)
    {
        indent();
        WITH_XTERM(1, 35, printf("{ // [%s]", name.c_str()));
        putchar('\n');
        ++depth;
    }

    void out(const std::string &name, const duration_t &d)
    {
        total_durations[name] += d;
        ++call_times[name];
        --depth;

        char buffer[128];
        sprintf(buffer, "[%s] took ", name.c_str());
        [&](double t) {
            if (t < 1) {
                sprintf(buffer + strlen(buffer), "%.2fms", t * 1000);
            } else {
                sprintf(buffer + strlen(buffer), "%.2fs", t);
            }
        }(d.count());
        indent();
        WITH_XTERM(1, 32, printf("} // %s", buffer));
        putchar('\n');
    }

    void indent(FILE *fp = stdout)
    {
        for (int i = 0; i < depth; ++i) { fprintf(fp, "    "); }
    }

    template <typename... Args> void logf1(FILE *fp, const Args &... args)
    {
        fprintf(fp, "// ");
        fprintf(fp, args...);
        fputc('\n', fp);
    }

    template <typename... Args> void logf(const Args &... args)
    {
        for (auto fp : log_files) {
            if (fp == stdout) { indent(fp); }
            logf1(fp, args...);
            break;  // only log to the first
        }
    }

    std::deque<FILE *> log_files;

  private:
    const std::string name;

    const std::chrono::time_point<clock_t> t0;

    int depth;

    std::map<std::string, duration_t> total_durations;
    std::map<std::string, uint32_t> call_times;

    void report(FILE *fp) const
    {
        const auto total = since<double, clock_t>(t0);
        using item_t = std::tuple<duration_t, uint32_t, std::string>;
        std::vector<item_t> list;
        // for (const auto [name, duration] : total_durations) {
        for (const auto &it : total_durations) {
            const auto name = it.first;
            const auto duration = it.second;
            list.push_back(item_t(duration, call_times.at(name), name));
        }
        std::sort(list.rbegin(), list.rend());

        const std::string hr(80, '-');
        fprintf(fp, "\tsummary of %s::%s (%fs)\n", "tracer_ctx_t",  //
                name.c_str(), total.count());
        fprintf(fp, "%s\n", hr.c_str());
        fprintf(fp, "%8s    %16s    %12s    %12s    %s\n",  //
                "count", "cumulative (s)", "%", "mean (ms)", "call site");
        fprintf(fp, "%s\n", hr.c_str());
        // for (const auto &[duration, count, name] : list) {
        for (const auto &it : list) {
            const auto duration = std::get<0>(it);
            const auto count = std::get<1>(it);
            const auto name = std::get<2>(it);
            fprintf(fp, "%8d    %16f    %12.2f    %12.4f    %s\n",  //
                    count, duration.count(), duration * 100 / total,
                    1000 * duration.count() / count, name.c_str());
        }
    }
};
