#pragma once
#include <algorithm>
#include <chrono>
#include <cstring>
#include <sstream>
#include <stack>
#include <string>

#include <tracer/bits/stdtracer_base.hpp>
#include <tracer/bits/stdtracer_xterm.hpp>

template <typename clock_t, typename duration_t>
class log_tracer_ctx_t_
{
    const std::string name;
    const std::chrono::time_point<clock_t> t0;

    int depth;
    FILE *fp_;
    std::deque<FILE *> log_files;

  public:
    explicit log_tracer_ctx_t_(const std::string &name, FILE *fp = stderr)
        : name(name), t0(clock_t::now()), depth(0), fp_(fp)
    {
        log_files.push_front(stdout);
    }

    void in(const std::string &name)
    {
        indent(fp_);
        WITH_XTERM(1, 35, std::fprintf(fp_, "{ // [%s]", name.c_str()));
        std::fputc('\n', fp_);
        ++depth;
    }

    void out(const std::string &name, const duration_t &d)
    {
        --depth;
        char buffer[512];
        std::snprintf(buffer, 400, "[%s", name.c_str());
        char *p = buffer + std::strlen(buffer);
        [&](double t) {
            if (t < 1) {
                std::sprintf(p, "] took %.2fms", t * 1000);
            } else {
                std::sprintf(p, "] took %.2fs", t);
            }
        }(d.count());
        indent(fp_);
        WITH_XTERM(1, 32, std::fprintf(fp_, "} // %s", buffer));
        std::fputc('\n', fp_);
    }

    void indent(FILE *fp)
    {
        for (int i = 0; i < depth; ++i) { std::fprintf(fp, "    "); }
    }

    template <typename... Args>
    void logf1(FILE *fp, const Args &... args)
    {
        std::fprintf(fp, "// ");
        std::fprintf(fp, args...);
        std::fputc('\n', fp);
    }

    template <typename... Args>
    void logf(const Args &... args)
    {
        for (auto fp : log_files) {
            if (fp == stdout) { indent(fp); }
            logf1(fp, args...);
            break;  // only log to the first
        }
    }
};
