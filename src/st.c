#include <stdio.h>
#include <stdarg.h>
#include <xc.h>

#include "../h/st.h"

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
