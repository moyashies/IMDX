#include "../h/protocol.h"

static unsigned int receivedIndex = 0;
static unsigned int receivingIndex = 0;
static unsigned int receiving[15] = {};

void clearReceivingBuffer()
{
    receiving[0] = 0;
    receiving[1] = 0;
    receiving[2] = 0;
    receiving[3] = 0;
    receiving[4] = 0;
    receiving[5] = 0;
    receiving[6] = 0;
    // ;(
    // receiving[7] = 0;
    // receiving[8] = 0;
    // receiving[9] = 0;
    // receiving[10] = 0;
    receiving[11] = 0;
    receiving[12] = 0;
    receiving[13] = 0;
    receiving[14] = 0;
    receivedIndex = 0;
    receivingIndex = 0;
}

int doneReceiving(unsigned int* received)
{
    if (receivedIndex & 0x000f) { /* required [a-f] parameters */
        received[0] = receiving[0];
        received[1] = receiving[1];
        received[2] = receiving[2];
        received[3] = receiving[3];
        received[4] = receiving[4];
        received[5] = receiving[5];
        received[6] = receiving[6];
        received[7] = receiving[7];
        received[8] = receiving[8];
        received[9] = receiving[9];
        received[10] = receiving[10];
        received[11] = receiving[11];
        received[12] = receiving[12];
        received[13] = receiving[13];
        received[14] = receiving[14];
        return 1;
    }
    return 0;
}

void prepareReceive(unsigned int c)
{
    receivingIndex = c - 'a';
    receivedIndex |= 1 << receivingIndex;
    receiving[receivingIndex] = 0;
}

void receiveNum(unsigned int c)
{
    unsigned int num;

    num = receiving[receivingIndex] * 10 + (c - '0');
    if (num < 256) {
        receiving[receivingIndex] = num;
    }
}
