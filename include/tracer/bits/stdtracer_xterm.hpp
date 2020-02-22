#pragma once
#include <cstdio>

#include <unistd.h>

struct xterm_t {
    const bool is_tty;

    xterm_t(uint8_t b, uint8_t f) : is_tty(isatty(fileno(stdout)))
    {
        if (is_tty) { std::printf("\e[%u;%um", b, f); }
    }

    ~xterm_t()
    {
        if (is_tty) { std::printf("\e[m"); }
    }
};

#define WITH_XTERM(b, f, e)                                                    \
    {                                                                          \
        xterm_t _(b, f);                                                       \
        e;                                                                     \
    }
