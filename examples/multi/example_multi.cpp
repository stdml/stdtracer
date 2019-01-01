#include <cstdio>

#include "trace.hpp"

int fibo(int n)
{
    // don't use in recursive function!
    if (n < 2) { return 1; }
    return fibo(n - 1) + fibo(n - 2);
}

void f(int n)
{
    TRACE_SCOPE(__func__);
    int x = TRACE_EXPR(fibo(n));
    printf("fibo(%d) = %d\n", n, x);
}

void g()
{
    TRACE_SCOPE(__func__);
    for (int i = 0; i < 43; ++i) { TRACE_STMT(f(i)); }
}

int main()
{
    TRACE_SCOPE(__func__);
    g();
    return 0;
}
