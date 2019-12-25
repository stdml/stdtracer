#include "testing.hpp"

std::string readfile(const char *filename)
{
    FILE *fp = std::fopen(filename, "r");
    std::string text;
    char line[1 << 16];
    while (std::fgets(line, (1 << 16) - 1, fp)) { text += line; }
    std::fclose(fp);
    return text;
}

void file_equal(const char *f, const char *g)
{
    const auto p = readfile(f);
    const auto q = readfile(g);
    ASSERT_EQ(p, q);
}

template <typename Context>
void test_report(const char *filename, const char *expected)
{
    Context ctx("global");
    {
        ctx.in("f");
        ctx.out("f", default_duration_t(.5));
    }
    {
        FILE *fp = std::fopen(filename, "w");
        ctx.report(fp, default_duration_t(1));
        std::fclose(fp);
    }
    file_equal(filename, expected);
}

TEST(suite, simple_tracer_ctx_t_report)
{
    test_report<simple_tracer_ctx_t>("simple_tracer_ctx_t_report.log",
                                     "testdata/simple.txt");
}

TEST(suite, stack_tracer_ctx_t_report)
{
    test_report<stack_tracer_ctx_t>("stack_tracer_ctx_t_report.log",
                                    "testdata/stack.txt");
}

TEST(suite, thread_tracer_ctx_t_report)
{
    test_report<thread_tracer_ctx_t>("thread_tracer_ctx_t_report.log",
                                     "testdata/thread.txt");
}
