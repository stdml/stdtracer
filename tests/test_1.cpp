#include <stdtracer>

void f() { TRACE(__func__); }

void g()
{
    TRACE(__func__);
    for (int i = 0; i < 10; ++i) { f(); }
}

int main()
{
    TRACE(__func__);
    g();
    return 0;
}
