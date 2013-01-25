#ifndef __RECEIVE_H
#define __RECEIVE_H

#define RX_BUFSIZE 16

enum rx_protocol {
   RX_ROTATE   = 1,
   RX_HANDLE_X = 2,
   RX_HANDLE_Y = 3,
   RX_THROTTLE = 4,
   RX_TRIGGER  = 5,

   RX_ANGLE_KP = 7,
   RX_ANGLE_KD = 8,
   RX_GYRO_KP  = 9,
   RX_GYRO_KD  = 10
};

struct rx {
    unsigned int buf[RX_BUFSIZE];
    unsigned char ok;
    unsigned char timeout;
};

extern struct rx rx;

void rxBegin();
void rxCommit();
void rxIndex(unsigned int c);
void rxNum(unsigned int c);

#endif /* __RECEIVE_H */
