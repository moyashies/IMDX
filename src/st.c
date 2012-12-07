#include "../h/st.h"

#define TRANSMIT_CHAR(__c) do { \
    while (U1STAbits.UTXBF); \
    U1TXREG = __c; \
} while (0)

void transmit_str(const char* buf)
{
    for (; ; ++buf) {
        if (*buf != '\0') {
            TRANSMIT_CHAR(*buf);
        } else {
            break;
        }
    }
}

void st(const char* buf)
{
    transmit_str(buf);
    TRANSMIT_CHAR('\r');
    TRANSMIT_CHAR('\n');
}

void st128(const char* buf, char num)
{
    transmit_str(buf);
    if (num < 0) {
        TRANSMIT_CHAR('-');
        num = -num;
    } else {
        TRANSMIT_CHAR(' ');
    }
    if (num == -128) { // XXX
        TRANSMIT_CHAR('1');
        TRANSMIT_CHAR('2');
        TRANSMIT_CHAR('8');
    } else {
        TRANSMIT_CHAR('0' + num / 100 % 10);
        TRANSMIT_CHAR('0' + num / 10 % 10);
        TRANSMIT_CHAR('0' + num % 10);
    }
}

void st255(const char* buf, unsigned char num)
{
    transmit_str(buf);
    TRANSMIT_CHAR('0' + num / 100 % 10);
    TRANSMIT_CHAR('0' + num / 10 % 10);
    TRANSMIT_CHAR('0' + num % 10);
}

void st32768(const char* buf, int num)
{
    transmit_str(buf);
    if (num < 0) {
        TRANSMIT_CHAR('-');
        num = -num;
    } else {
        TRANSMIT_CHAR(' ');
    }
    TRANSMIT_CHAR('0' + num / 10000 % 10);
    TRANSMIT_CHAR('0' + num / 1000 % 10);
    TRANSMIT_CHAR('0' + num / 100 % 10);
    TRANSMIT_CHAR('0' + num / 10 % 10);
    TRANSMIT_CHAR('0' + num % 10);
}

void stff(const char* buf, unsigned char num)
{
    transmit_str(buf);
    TRANSMIT_CHAR('0');
    TRANSMIT_CHAR('x');
    if (num > 0x9f) {
        TRANSMIT_CHAR('W' + num / 16);
    } else {
        TRANSMIT_CHAR('0' + num / 16);
    }
    if ((num & 0x0f) > 0x09) {
        TRANSMIT_CHAR('W' + (num & 0xf));
    } else {
        TRANSMIT_CHAR('0' + num % 16);
    }
    TRANSMIT_CHAR('\r');
    TRANSMIT_CHAR('\n');
}
