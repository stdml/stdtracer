#include <tracer/site>

int fibo(int n)
{
    // don't use in recursive function!
    if (n < 2) { return 1; }
    return fibo(n - 1) + fibo(n - 2);
}

void f(int n)
{
    TRACE_SITE_SCOPE(__func__);
    int x = TRACE_SITE_EXPR(fibo(n));
    printf("fibo(%d) = %d\n", n, x);
}

void g()
{
    TRACE_SITE_SCOPE(__func__);
    for (int i = 0; i < 43; ++i) { TRACE_SITE_STMT(f(i)); }
}

int main()
{
    TRACE_SITE_SCOPE(__func__);
    g();
    return 0;
}
