#include <stdio.h>
#include <stdarg.h>
#ifndef TEST_IMDX
#  include <xc.h>
#endif
#include "../h/transmit.h"

#define TX_BUFSIZE 128

struct tx {
    char buf[TX_BUFSIZE];
    unsigned char num;
    unsigned char index;
};

static struct tx tx = {"", 0, 0};

char txPop()
{
    char c;

    c = tx.buf[tx.index % TX_BUFSIZE];
    tx.index++;
    tx.num--;

    return c;
}

void txPush(char c)
{
    while (tx.num == TX_BUFSIZE);

    tx.buf[(tx.index + tx.num) % TX_BUFSIZE] = c;
    tx.num++;
#ifndef TEST_IMDX
    if (!IEC0bits.U1TXIE) {
        U1TXREG = txPop();
        IFS0bits.U1TXIF = 0;
        IEC0bits.U1TXIE = 1;
    }
#endif
}

unsigned char txIsEmpty()
{
    return tx.num == 0;
}

void transmitStr(const char *buf)
{
    for (; ; ++buf) {
        if (*buf != '\0') {
            txPush(*buf);
        } else {
            break;
        }
    }
}

void stf(const char *format, ...)
{
    char buf[1024];
    va_list args;

    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);

    transmitStr(buf);
}
