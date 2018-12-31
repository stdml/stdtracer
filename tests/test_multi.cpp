#include <vector>

#include "testing.hpp"

template <int i, typename duration_t = default_duration_t> class fake_context
{
    std::vector<int> &_events;

  public:
    fake_context(std::vector<int> &events) : _events(events) {}

    void in(const std::string &name) { _events.push_back(i); }

    void out(const std::string &name, const duration_t &d)
    {
        _events.push_back(-i);
    }
};

using fake_tracer_t = multi_ctx_scope_t_<std::chrono::high_resolution_clock,  //
                                         fake_context<1>,                     //
                                         fake_context<2>,                     //
                                         fake_context<3>                      //
                                         >;

TEST(suite, test1)
{
    std::vector<int> events;
    fake_context<1> c1(events);
    fake_context<2> c2(events);
    fake_context<3> c3(events);
    {
        fake_tracer_t _("s1", c1, c2, c3);
        {
            fake_tracer_t _("s1", c1, c2, c3);
        }
    }
    {
        fake_tracer_t _("s1", c1, c2, c3);
    }

    std::vector<int> expect(
        {1, 2, 3, 1, 2, 3, -3, -2, -1, -3, -2, -1, 1, 2, 3, -3, -2, -1});
    ASSERT_EQ(expect.size(), events.size());
    for (int i = 0; i < events.size(); i++) { ASSERT_EQ(expect[i], events[i]); }
}
