// TODO: show invoke tree
#include <stdtracer.hpp>

using tracer_t = scope_t_<stack_tracer_ctx_t>;

stack_tracer_ctx_t default_stack_tracer_ctx("global");

#define TRACE(name) tracer_t _((name), default_stack_tracer_ctx)

int fibo(int n)
{
    // don't use in recursive function!
    if (n < 2) { return 1; }
    return fibo(n - 1) + fibo(n - 2);
}

void work(int n)
{
    TRACE(__func__);
    int x = fibo(n);
    printf("fibo(%d) = %d\n", n, x);
}

void f()
{
    TRACE(__func__);
    for (int i = 0; i < 20; ++i) { work(i); }
}

void g()
{
    TRACE(__func__);
    for (int i = 20; i < 30; ++i) { work(i); }
}

void h()
{
    TRACE(__func__);
    for (int i = 30; i < 43; ++i) { work(i); }
}

int main()
{
    TRACE(__func__);
    f();
    g();
    h();
    return 0;
}
