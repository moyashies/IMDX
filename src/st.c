#include <stdio.h>
#include <stdarg.h>
#include <xc.h>

#include "../h/st.h"

void transmitStr(const char* buf)
{
    for (; ; ++buf) {
        if (*buf != '\0') {
            setChar(*buf);
        } else {
            break;
        }
    }
}

void st(const char* buf)
{
    transmitStr(buf);
}

void stf(const char* format, ...)
{
    char buf[1024];
    va_list args;

    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);

    transmitStr(buf);
}
