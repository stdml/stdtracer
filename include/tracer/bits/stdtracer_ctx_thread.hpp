#pragma once
#include <algorithm>
#include <cstdio>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <tracer/bits/stdtracer_base.hpp>

template <typename clock_t, typename duration_t, bool no_report = false>
class thread_tracer_ctx_t_
{
    using instant_t = std::chrono::time_point<clock_t>;

    const std::string name;
    const instant_t t0;
    const std::string report_file;
    const bool report_stdout_;

    using call_info_t = std::pair<uint32_t, duration_t>;
    using call_info_map_t = std::unordered_map<std::string, call_info_t>;

    call_info_map_t call_info_map;

    std::mutex mu;

  public:
    explicit thread_tracer_ctx_t_(const std::string &name,
                                  const std::string &report_file = "")
        : name(name), t0(clock_t::now()), report_file(report_file),
          report_stdout_(report_stdout())
    {
    }

    ~thread_tracer_ctx_t_()
    {
        const auto total = since<double, clock_t>(t0);
        if (no_report) { return; }
        if (call_info_map.empty()) { return; }
        if (!report_file.empty()) {
            FILE *fp = std::fopen(report_file.c_str(), "w");
            report(fp, total);
            std::fclose(fp);
            std::fprintf(stderr, "// profile info logged to file://%s\n",
                         report_file.c_str());
        }
        if (report_stdout_) { report(stdout, total); }
    }

    void in(const std::string &name) {}

    void out(const std::string &name, const duration_t &d)
    {
        std::lock_guard<std::mutex> lk(mu);

        auto &info = call_info_map[name];
        ++info.first;
        info.second += d;
    }

    void report(FILE *fp, const duration_t &total) const
    {
        report(call_info_map, total, name, fp);
    }

  private:
    static void report(const call_info_map_t &call_info_map,
                       const duration_t &total, const std::string &name,
                       FILE *fp)
    {
        using item_t = std::tuple<duration_t, uint32_t, std::string>;
        std::vector<item_t> list;
        // for (const auto [name, duration] : total_durations) {
        for (const auto &it : call_info_map) {
            const auto name = it.first;
            const auto call_times = it.second.first;
            const auto duration = it.second.second;
            list.push_back(item_t(duration, call_times, name));
        }
        std::sort(list.rbegin(), list.rend());

        const std::string hr(80, '-');
        std::fprintf(fp, "\tsummary of %s::%s (%fs)\n", "thread_tracer_ctx",  //
                     name.c_str(), total.count());
        std::fprintf(fp, "%s\n", hr.c_str());
        std::fprintf(fp, header_fmt,  //
                     "#", "count", "cumulative (s)", "%", "mean (ms)",
                     "call site");
        std::fprintf(fp, "%s\n", hr.c_str());
        int idx = 0;
        for (const auto &it : list) {
            const auto duration = std::get<0>(it);
            const auto count = std::get<1>(it);
            const auto name = std::get<2>(it);
            std::fprintf(fp, row_fmt,  //
                         ++idx, count, duration.count(), duration * 100 / total,
                         1000 * duration.count() / count, name.c_str());
        }
    }
};
