#pragma once
#include <chrono>
#include <cstring>
#include <string>

#include <tracer/bits/stdtracer_base.hpp>
#include <tracer/bits/stdtracer_xterm.hpp>

class auto_binary_ratio
{
    static constexpr const char *One = "";
    static constexpr const char *Kili = "Ki";
    static constexpr const char *Megi = "Mi";
    static constexpr const char *Gigi = "Gi";

    static constexpr const int64_t ki = 1 << 10;
    static constexpr const int64_t mi = ki << 10;
    static constexpr const int64_t gi = mi << 10;

    using P = std::pair<double, const char *>;

  public:
    P operator()(double n) const
    {
        if (n >= gi) { return P(n / gi, Gigi); }
        if (n >= mi) { return P(n / mi, Megi); }
        if (n >= ki) { return P(n / ki, Kili); }
        return P(n, One);
    }
};

template <typename D, typename N>
class rate_reporter_t_
{
    const std::string name_;
    const std::string unit_;
    FILE *fp_;
    bool report_;

  public:
    explicit rate_reporter_t_(std::string name, std::string unit,
                              FILE *fp = stderr)
        : name_(std::move(name)), unit_(std::move(unit)), fp_(fp),
          report_(report_stdout())
    {
    }

    void operator()(D d, N n, const char *prefix) const
    {
        if (!report_) { return; }
        const double r = static_cast<double>(n) / d.count();
        if (unit_ == "B") {
            const auto [rr, uu] = auto_binary_ratio()(r);
            fprintf(fp_, "%s%.3f %s%s/sec | %s\n", prefix, rr, uu,
                    unit_.c_str(), name_.c_str());
        } else {
            fprintf(fp_, "%s%.2f %s/sec | %s\n", prefix, r, unit_.c_str(),
                    name_.c_str());
        }
    }

    void operator()(D d, N n) const { (*this)(d, n, "%%%% "); }
};

class rate_ctx_t_
{
    using D = std::chrono::duration<double>;
    using N = int64_t;
    using reporter_t = rate_reporter_t_<D, N>;

    reporter_t reporter_;

    N amount_;
    D duration_;

  public:
    explicit rate_ctx_t_(std::string name, std::string unit, FILE *fp = stderr)
        : reporter_(std::move(name), std::move(unit), fp)
    {
    }

    ~rate_ctx_t_() { reporter_(duration_, amount_, "[overall rate]: "); }

    void in() {}

    void out(const D &d, int64_t n)
    {
        reporter_(d, n, "[rate]: ");
        amount_ += n;
        duration_ += d;
    }
};

class rate_scope_t_
{
    rate_ctx_t_ &ctx_;
    using N = int64_t;

    using C = std::chrono::high_resolution_clock;
    using T = std::chrono::time_point<C>;

    T t0;
    N n;

  public:
    rate_scope_t_(rate_ctx_t_ &ctx, N n) : ctx_(ctx), t0(C::now()), n(n)
    {
        ctx_.in();
    }

    ~rate_scope_t_() { ctx_.out(C::now() - t0, n); }
};
