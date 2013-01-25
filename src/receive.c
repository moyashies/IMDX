#include <string.h>
#include "../h/receive.h"

static unsigned int receiving[RX_BUFSIZE];
static unsigned int receivingIndex;
static unsigned int received;

void rxBegin()
{
    rx.ok = 0;
    memset(receiving, 0, sizeof(receiving));
    received = 0;
    receivingIndex = 0;
}

void rxCommit()
{
    /* [b-e] parameter is required. */
    if (received & 0x001e) {
        memcpy(rx.buf, receiving, sizeof(receiving));
        rx.ok = 1;
    }
}

void rxIndex(unsigned int c)
{
    receivingIndex = c - 'a';
    received |= 1 << (receivingIndex + 1);
    receiving[receivingIndex] = 0;
}

void rxNum(unsigned int c)
{
    unsigned int num;

    num = receiving[receivingIndex] * 10 + (c - '0');
    if (num < 256) {
        receiving[receivingIndex] = num;
    }
}
