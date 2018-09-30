#pragma once
#include <algorithm>
#include <chrono>
#include <map>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

template <typename clock_t, typename duration_t> class stack_tracer_ctx_t_
{
    template <typename T>
    std::chrono::duration<T> since(const std::chrono::time_point<clock_t> &t0)
    {
        return clock_t::now() - t0;
    }

  public:
    stack_tracer_ctx_t_(const std::string &name)
        : name(name), t0(clock_t::now()), call_stack_str("")
    {
    }

    ~stack_tracer_ctx_t_() { report(stdout); }

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

    void report(FILE *fp)
    {
        const auto total = since<double>(t0);
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
        fprintf(fp, "\tinvoke tree of %s::%s (%fs)\n", "stack_tracer_ctx_t_",
                name.c_str(), total.count());
        fprintf(fp, "%s\n", hr.c_str());
        fprintf(fp, "%8s    %16s    %12s    %12s    %s\n",  //
                "count", "cumulative (s)", "%", "mean (ms)", "call chain");
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

  private:
    const std::string name;
    const std::chrono::time_point<clock_t> t0;

    std::string call_stack_str;

    std::map<std::string, int> index;
    std::vector<std::string> names;

    using call_info_t = std::pair<uint32_t, duration_t>;
    std::map<std::string, call_info_t> call_info_map;

    int get_idx(const std::string &name)
    {
        const auto pos = index.find(name);
        if (pos != index.end()) { return pos->second; }
        const int idx = names.size();
        names.push_back(name);
        index[name] = idx;
        return idx;
    }
};

using stack_tracer_ctx_t =
    stack_tracer_ctx_t_<std::chrono::high_resolution_clock,
                        std::chrono::duration<double>>;
