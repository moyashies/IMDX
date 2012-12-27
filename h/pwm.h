#ifndef __PWM_H
#define __PWM_H

#define MIN(__a, __b) ((__a) < (__b) ? (__a) : (__b))
#define MAX(__a, __b) ((__a) > (__b) ? (__a) : (__b))

#define PWM_BASE 21000
#define PWM_L_BASE 20860
#define PWM_R_BASE 20860
#define PWM_F_BASE PWM_BASE
#define PWM_B_BASE PWM_BASE

#define PWM_MAX 28000
#define PWM_MIN 20000
#define PWM_STOP 18000

#define PWM_THROTTLE 8
#define PWM_HANDLE 2
#define PWM_POSTURE 16
#define PWM_ROTATE 4

#define ACCE_KP 20
#define ACCE_KD 12
#define GYRO_KP 12
#define GYRO_KD 16

unsigned int PWMLeft(const unsigned int* received, int gyroZ, int angleXPD, int gyroXPD);
unsigned int PWMRight(const unsigned int* received, int gyroZ, int angleXPD, int gyroXPD);
unsigned int PWMFront(const unsigned int* received, int gyroZ, int angleYPD, int gyroYPD);
unsigned int PWMBack(const unsigned int* received, int gyroZ, int angleYPD, int gyroYPD);

int _my_angleXPD(const int* angle, const int* angleBefore, int kp, int kd);
int _my_angleYPD(const int* angle, const int* angleBefore, int kp, int kd);
int _my_gyroXPD(const int* gyro, const int* gyroBefore, int kp, int kd);
int _my_gyroYPD(const int* gyro, const int* gyroBefore, int kp, int kd);

#endif /* __PWM_H */
