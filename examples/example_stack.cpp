#include <cstdio>

#include <tracer/stack>

DEFINE_TRACE_CONTEXTS;  // must be defined exactly once in one executable

int fibo(int n)
{
    // don't use in deep recursive function!
    if (n < 2) { return 1; }
    return fibo(n - 1) + fibo(n - 2);
}

void work(int n)
{
    TRACE_SCOPE(__func__);
    int x = TRACE_EXPR(fibo(n));
    printf("fibo(%d) = %d\n", n, x);
}

void f()
{
    TRACE_SCOPE(__func__);
    for (int i = 0; i < 20; ++i) { TRACE_STMT(work(i)); }
}

void g()
{
    TRACE_SCOPE(__func__);
    for (int i = 20; i < 30; ++i) { TRACE_STMT(work(i)); }
}

void h()
{
    TRACE_SCOPE(__func__);
    for (int i = 30; i < 43; ++i) { TRACE_STMT(work(i)); }
}

int main()
{
    TRACE_SCOPE(__func__);
    f();
    g();
    h();
    return 0;
}
