#include <stdio.h>
#include <stdarg.h>
#include <xc.h>

#include "../h/st.h"

__inline__ static void transmitChar(const char c)
{
    while (U1STAbits.UTXBF);
    U1TXREG = c;
}

void transmitStr(const char* buf)
{
    for (; ; ++buf) {
        if (*buf != '\0') {
            transmitChar(*buf);
        } else {
            break;
        }
    }
}

void st(const char* buf)
{
    transmitStr(buf);
    transmitChar('\n');
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
