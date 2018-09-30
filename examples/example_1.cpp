#include <stdtracer>

int fibo(int n)
{
    // don't use in recursive function!
    if (n < 2) { return 1; }
    return fibo(n - 1) + fibo(n - 2);
}

void f(int n)
{
    TRACE(__func__);
    int x = fibo(n);
    printf("fibo(%d) = %d\n", n, x);
}

void g()
{
    TRACE(__func__);
    for (int i = 0; i < 43; ++i) { f(i); }
}

int main()
{
    TRACE(__func__);
    g();
    return 0;
}
