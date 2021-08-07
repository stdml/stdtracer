#pragma once
#include <cstdio>

#ifdef HAVE_UNISTD
#    include <unistd.h>

inline bool std_isatty(FILE *fp) { return isatty(fileno(fp)); }
#else
inline bool std_isatty(FILE *fp) { return false; }
#endif

class xterm_t
{
    uint8_t b;
    uint8_t f;

  public:
    xterm_t(uint8_t b, uint8_t f) : b(b), f(f) {}

    const char *operator()(const char *s) const
    {
        static char line[1 << 8];
        std::sprintf(line, "\e[%u;%um%s\e[m", b, f, s);
        return line;
    }
};

inline const xterm_t xt_red(1, 31);
inline const xterm_t xt_green(1, 32);
inline const xterm_t xt_yellow(1, 33);

struct with_xterm_t {
    const bool is_tty;

    with_xterm_t(uint8_t b, uint8_t f) : is_tty(std_isatty(stdout))
    {
        if (is_tty) { std::fprintf(stdout, "\e[%u;%um", b, f); }
    }

    ~with_xterm_t()
    {
        if (is_tty) { std::fprintf(stdout, "\e[m"); }
    }
};

#define WITH_XTERM(b, f, e)                                                    \
    {                                                                          \
        with_xterm_t _(b, f);                                                  \
        e;                                                                     \
    }
