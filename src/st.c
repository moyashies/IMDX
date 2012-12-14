#include <stdio.h>
#include <stdarg.h>
#include <xc.h>

#include "../h/st.h"

__inline__ static void transmit_char(const char c)
{
    while (U1STAbits.UTXBF);
    U1TXREG = c;
}

void transmit_str(const char* buf)
{
    for (; ; ++buf) {
        if (*buf != '\0') {
            transmit_char(*buf);
        } else {
            break;
        }
    }
}

void st(const char* buf)
{
    transmit_str(buf);
    transmit_char('\n');
}

void stf(const char* format, ...)
{
    char buf[1024];
    va_list args;

    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);

    st(buf);
}

void st128(const char* buf, char num)
{
    transmit_str(buf);
    if (num < 0) {
        transmit_char('-');
        num = -num;
    } else {
        transmit_char(' ');
    }
    if (num == -128) { // XXX
        transmit_char('1');
        transmit_char('2');
        transmit_char('8');
    } else {
        transmit_char('0' + num / 100 % 10);
        transmit_char('0' + num / 10 % 10);
        transmit_char('0' + num % 10);
    }
}

void st255(const char* buf, unsigned char num)
{
    transmit_str(buf);
    transmit_char('0' + num / 100 % 10);
    transmit_char('0' + num / 10 % 10);
    transmit_char('0' + num % 10);
}

void st32768(const char* buf, int num)
{
    transmit_str(buf);
    if (num < 0) {
        transmit_char('-');
        num = -num;
    } else {
        transmit_char(' ');
    }
    transmit_char('0' + num / 10000 % 10);
    transmit_char('0' + num / 1000 % 10);
    transmit_char('0' + num / 100 % 10);
    transmit_char('0' + num / 10 % 10);
    transmit_char('0' + num % 10);
}

void stff(const char* buf, unsigned char num)
{
    transmit_str(buf);
    transmit_char('0');
    transmit_char('x');
    if (num > 0x9f) {
        transmit_char('W' + num / 16);
    } else {
        transmit_char('0' + num / 16);
    }
    if ((num & 0x0f) > 0x09) {
        transmit_char('W' + (num & 0xf));
    } else {
        transmit_char('0' + num % 16);
    }
    transmit_char('\r');
    transmit_char('\n');
}
