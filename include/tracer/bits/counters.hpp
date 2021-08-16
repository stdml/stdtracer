
#pragma once
#include <limits>
#include <utility>

template <typename T, T unit, typename F>
struct basic_monoid_accumulator_t {
    using S = T;

    static constexpr S init = unit;

    T operator()(T acc, const T &x) const { return F()(acc, x); }

    T operator()(T acc) const { return acc; }
};

template <typename T>
struct count_accumulator {
    using S = T;

    static constexpr S init = 0;

    template <typename V>
    S operator()(S acc, const V &) const
    {
        return acc + 1;
    }

    T operator()(S acc) const { return acc; }
};

template <typename T>
using sum_accumulator = basic_monoid_accumulator_t<T, 0, std::plus<T>>;

template <typename T>
struct std_min {
    T operator()(const T &x, const T &y) const { return std::min(x, y); }
};

template <typename T>
struct std_max {
    T operator()(const T &x, const T &y) const { return std::max(x, y); }
};

template <typename T>
using min_accumulator =
    basic_monoid_accumulator_t<T, std::numeric_limits<T>::max(), std_min<T>>;

template <typename T>
using max_accumulator =
    basic_monoid_accumulator_t<T, std::numeric_limits<T>::min(), std_max<T>>;

template <typename T, typename N = uint32_t>
struct mean_accumulator {
    using S = std::pair<T, N>;

    static constexpr S init = std::make_pair(T(0), 0);

    S operator()(const S &acc, const T &x) const
    {
        return S(acc.first + x, acc.second + 1);
    }

    T operator()(const S &acc) const { return acc.first / acc.second; }
};

template <typename T, typename A>
class basic_counter
{
    typename A::S x;

  public:
    basic_counter() : x(A::init) {}

    void operator()(T y) { x = A()(x, y); }

    operator T() const { return A()(x); }
};
