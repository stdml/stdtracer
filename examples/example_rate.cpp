#include <tracer/rate>

int fibo_fast(int n)
{
    if (n < 2) { return 1; }
    int a = 1;
    int b = 1;
    for (int i = 2; i <= n; ++i) {
        a += b;
        std::swap(a, b);
    }
    return b;
}

int fibo(int n)
{
    if (n < 2) { return 1; }
    return fibo(n - 1) + fibo(n - 2);
}

void f(int n)
{
    int x = fibo_fast(n);
    TRACE_SITE_SCOPE_RATE(__func__, "rec", x);
    int y = fibo(n);
    if (x != y) { exit(1); }
    printf("fibo(%d) = %d\n", n, y);
}

void f2(int n)
{
    int x = fibo_fast(n);
    TRACE_SITE_SCOPE_RATE(__func__, "B", x);
    int y = fibo(n);
    if (x != y) { exit(1); }
    printf("fibo(%d) = %d\n", n, y);
}

int main()
{
    for (int i = 0; i < 43; ++i) {
        f(i);
        f2(i);
    }
    return 0;
}
