#ifndef __RECEIVE_H
#define __RECEIVE_H

#define RX_BUFSIZE 16

enum rx_protocol {
   RX_ROTATE   = 1, /* b */
   RX_HANDLE_X = 2, /* c */
   RX_HANDLE_Y = 3, /* d */
   RX_THROTTLE = 4, /* e */
   RX_TRIGGER  = 5, /* f */

   RX_ANGLE_KP = 6, /* g */
   RX_ANGLE_KD = 7, /* h */
   RX_GYRO_KP  = 8, /* i */
   RX_GYRO_KD  = 9, /* j */

   RX_ANGLE_GAIN = 10, /* k */
   RX_GYRO_GAIN  = 11, /* l */

   RX_MOTOR_L_BASE = 12, /* m */
   RX_MOTOR_R_BASE = 13, /* n */
   RX_MOTOR_F_BASE = 14, /* o */
   RX_MOTOR_B_BASE = 15  /* p */
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
