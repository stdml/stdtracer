#pragma once
#include <limits>
#include <utility>

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
struct sum_accumulator {
    using S = T;

    static constexpr S init = 0;

    T operator()(T acc, const T &x) const { return acc + x; }

    T operator()(T acc) const { return acc; }
};

template <typename T>
struct min_accumulator {
    using S = T;

    static constexpr S init = std::numeric_limits<T>::max();

    T operator()(T acc, const T &x) const { return std::min(acc, x); }

    T operator()(T acc) const { return acc; }
};

template <typename T>
struct max_accumulator {
    using S = T;

    static constexpr S init = std::numeric_limits<T>::lowest();

    T operator()(T acc, const T &x) const { return std::max(acc, x); }

    T operator()(T acc) const { return acc; }
};

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
