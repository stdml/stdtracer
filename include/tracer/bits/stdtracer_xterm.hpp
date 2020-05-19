#pragma once
#include <cstdio>

#ifdef HAVE_UNISTD
#    include <unistd.h>

inline bool std_isatty(FILE *fp) { return isatty(fileno(fp)); }
#else
inline bool std_isatty(FILE *fp) { return false; }
#endif

struct xterm_t {
    const bool is_tty;

    xterm_t(uint8_t b, uint8_t f) : is_tty(std_isatty(stdout))
    {
        if (is_tty) { std::fprintf(stdout, "\e[%u;%um", b, f); }
    }

    ~xterm_t()
    {
        if (is_tty) { std::fprintf(stdout, "\e[m"); }
    }
};

#define WITH_XTERM(b, f, e)                                                    \
    {                                                                          \
        xterm_t _(b, f);                                                       \
        e;                                                                     \
    }
