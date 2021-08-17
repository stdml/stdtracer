#include <cstdio>
#include <random>

#include <tracer/patient>

int fibo(int n)
{
    if (n < 2) { return 1; }
    return fibo(n - 1) + fibo(n - 2);
}

void f(int n)
{
    int x = fibo(n);
    printf("fibo(%d) = %d\n", n, x);
}

void g(int n = 41)
{
    std::random_device rd;
    std::mt19937 gen32(rd());
    WITH_PATIENT(__func__, 1);
    for (int i = 0; i < 10; ++i) {
        ASK_PATIENT_("computing fibo %d/%d", i, 10);
        f(n + gen32() % 2);
    }
}

int main()
{
    g();
    g(1);
    return 0;
}
