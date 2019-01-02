#pragma once
#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

#include <bits/stdtracer_base.hpp>

template <typename clock_t, typename duration_t, bool no_report = false>
class stat_tracer_ctx_t_
{
    using instant_t = std::chrono::time_point<clock_t>;

    const std::string name;
    const instant_t t0;
    const std::string report_file;

  public:
    explicit stat_tracer_ctx_t_(const std::string &name,
                                const std::string &report_file = "")
        : name(name), t0(clock_t::now()), report_file(report_file)
    {
    }

    ~stat_tracer_ctx_t_()
    {
        const auto total = since<double, clock_t>(t0);
        if (no_report) { return; }
        if (call_info_map.empty()) { return; }
        if (!report_file.empty()) {
            FILE *fp = fopen(report_file.c_str(), "w");
            report(fp, total);
            fclose(fp);
            fprintf(stderr, "// profile info logged to file://%s\n",
                    report_file.c_str());
        }
        report(stdout, total);
    }

    void in(const std::string &name) {}

    void out(const std::string &name, const duration_t &d)
    {
        auto &info = call_info_map[name];
        const auto count = ++std::get<0>(info);
        std::get<1>(info) += d;
        if (count == 1) {
            std::get<2>(info) = d;
            std::get<3>(info) = d;
        } else {
            std::get<2>(info) = std::min(std::get<2>(info), d);
            std::get<3>(info) = std::max(std::get<3>(info), d);
        }
    }

  private:
    using call_info_t =
        // (count, sum, min, max)
        std::tuple<uint32_t, duration_t, duration_t, duration_t>;

    std::unordered_map<std::string, call_info_t> call_info_map;

    void report(FILE *fp, const duration_t &total) const
    {
        // (sum, count, name, min, max)
        using item_t = std::tuple<duration_t, uint32_t, std::string, duration_t,
                                  duration_t>;
        std::vector<item_t> list;
        for (const auto &it : call_info_map) {
            const auto name = it.first;
            const auto count = std::get<0>(it.second);
            const auto sum = std::get<1>(it.second);
            const auto min = std::get<2>(it.second);
            const auto max = std::get<3>(it.second);
            list.push_back(item_t(sum, count, name, min, max));
        }
        std::sort(list.rbegin(), list.rend());

        const std::string hr(112, '-');
        fprintf(fp, "\tsummary of %s::%s (%fs)\n", "stat_tracer_ctx",  //
                name.c_str(), total.count());
        fprintf(fp, "%s\n", hr.c_str());

        constexpr const char *header_fmt =
            "%5s  %8s    %16s    %12s    %12s    %12s    %12s    %s\n";
        constexpr const char *row_fmt =
            "%5d  %8d    %16f    %12.2f    %12.4f    %12.4f    %12.4f    %s\n";

        fprintf(fp, header_fmt,  //
                "#", "count", "cumulative (s)", "%", "mean (ms)", "min (ms)",
                "max (ms)", "call site");
        fprintf(fp, "%s\n", hr.c_str());

        int idx = 0;
        for (const auto &it : list) {
            const auto sum = std::get<0>(it);
            const auto count = std::get<1>(it);
            const auto name = std::get<2>(it);
            const auto min = std::get<3>(it);
            const auto max = std::get<4>(it);
            fprintf(fp, row_fmt,  //
                    ++idx, count, sum.count(), sum * 100 / total,
                    1000 * sum.count() / count, 1000 * min.count(),
                    1000 * max.count(), name.c_str());
        }
    }
};
