#ifndef __PROTOCOL_H
#define __PROTOCOL_H

void clearReceivingBuffer();
int doneReceiving(unsigned int* received);
void prepareReceive(unsigned int c);
void receiveNum(unsigned int c);

#endif /* __PROTOCOL_H */
