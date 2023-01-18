#include <thread>
#include <vector>

#include <tracer/thread_log>

DEFINE_TRACE_CONTEXTS;  // must be defined exactly once in one executable

int fibo(int n)
{
    // don't use in deep recursive function!
    if (n < 2) { return 1; }
    return fibo(n - 1) + fibo(n - 2);
}

void f(int n, int &result)
{
    char name[64];
    snprintf(name, 64, "%s(%d)", __func__, n);
    TRACE_SCOPE(name);
    result = fibo(n);
}

void g()
{
    TRACE_SCOPE(__func__);
    const int n = 43;
    std::vector<int> fs(n);
    std::vector<std::thread> ths;
    for (int i = 0; i < n; ++i) {
        ths.push_back(std::thread([&, i = i] { f(i, fs[i]); }));
    }
    for (auto &th : ths) { th.join(); }
}

int main()
{
    TRACE_SCOPE(__func__);
    g();
    return 0;
}
