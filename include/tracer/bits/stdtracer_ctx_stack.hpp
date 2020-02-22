#pragma once
#include <algorithm>
#include <chrono>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <tracer/bits/stdtracer_base.hpp>

template <typename clock_t, typename duration_t, bool no_report = false>
class stack_tracer_ctx_t_
{
    using instant_t = std::chrono::time_point<clock_t>;

    const std::string name;
    const instant_t t0;
    const std::string report_file;
    const bool report_stdout_;

    std::string call_stack_str;

    std::unordered_map<std::string, int> index;
    std::vector<std::string> names;

    using call_info_t = std::pair<uint32_t, duration_t>;
    using call_info_map_t = std::unordered_map<std::string, call_info_t>;

    call_info_map_t call_info_map;

  public:
    stack_tracer_ctx_t_(const std::string &name,
                        const std::string &report_file = "")
        : name(name), t0(clock_t::now()), report_file(report_file),
          report_stdout_(report_stdout()), call_stack_str("")
    {
    }

    ~stack_tracer_ctx_t_()
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
        if (report_stdout_) { report(stdout, total); }
    }

    void in(const std::string &name)
    {
        const int idx = get_idx(name);
        call_stack_str += "/" + std::to_string(idx);
    }

    void out(const std::string &name, const duration_t &d)
    {
        auto &info = call_info_map[call_stack_str];
        ++info.first;
        info.second += d;

        call_stack_str.erase(call_stack_str.rfind('/'));
    }

    void report(FILE *fp, const duration_t &total) const
    {
        report(call_info_map, names, total, name, fp);
    }

  private:
    static void report(const call_info_map_t &call_info_map,
                       const std::vector<std::string> &names,
                       const duration_t &total, const std::string &name,
                       FILE *fp)
    {
        using item_t = std::tuple<duration_t, uint32_t, std::string>;
        std::vector<item_t> list;
        // for (const auto [name, duration] : total_durations) {
        for (const auto &it : call_info_map) {
            const auto name = it.first;
            const auto info = it.second;
            list.push_back(item_t(info.second, info.first, name));
        }
        std::sort(list.rbegin(), list.rend());

        const std::string hr(80, '-');
        fprintf(fp, "\tinvoke tree of %s::%s (%fs)\n", "stack_tracer_ctx_t",
                name.c_str(), total.count());
        fprintf(fp, "%s\n", hr.c_str());
        fprintf(fp, header_fmt,  //
                "#", "count", "cumulative (s)", "%", "mean (ms)", "call chain");
        fprintf(fp, "%s\n", hr.c_str());
        int idx = 0;
        // for (const auto &[duration, count, name] : list) {
        for (const auto &it : list) {
            const auto duration = std::get<0>(it);
            const auto count = std::get<1>(it);
            const auto name = decode_call_stack_str(std::get<2>(it), names);
            fprintf(fp, row_fmt,  //
                    ++idx, count, duration.count(), duration * 100 / total,
                    1000 * duration.count() / count, name.c_str());
        }
    }

    int get_idx(const std::string &name)
    {
        const auto pos = index.find(name);
        if (pos != index.end()) { return pos->second; }
        const int idx = names.size();
        names.push_back(name);
        index[name] = idx;
        return idx;
    }

    static std::vector<std::string> split(const std::string &text,
                                          const char sep)
    {
        std::vector<std::string> lines;
        std::string line;
        std::istringstream ss(text);
        while (std::getline(ss, line, sep)) {
            if (!line.empty()) { lines.push_back(line); }
        }
        return lines;
    }

    static std::string
    decode_call_stack_str(const std::string &call_ss,
                          const std::vector<std::string> &names)
    {
        std::string ss;
        for (const auto &s : split(call_ss, '/')) {
            const int idx = std::stoi(s);
            ss += "/" + names.at(idx);
        }
        return ss;
    }
};
