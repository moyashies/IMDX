#ifndef __MOTOR_H
#define __MOTOR_H

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

struct pwm {
    int left;
    int right;
    int front;
    int back;
};

extern struct pwm motor;

void motorStop();

void motorLeft(int gyroZ, int angleXPD, int gyroXPD);
void motorRight(int gyroZ, int angleXPD, int gyroXPD);
void motorFront(int gyroZ, int angleYPD, int gyroYPD);
void motorBack(int gyroZ, int angleYPD, int gyroYPD);

int _my_angleXPD(const int* angle, const int* angleBefore, int kp, int kd);
int _my_angleYPD(const int* angle, const int* angleBefore, int kp, int kd);
int _my_gyroXPD(const int* gyro, const int* gyroBefore, int kp, int kd);
int _my_gyroYPD(const int* gyro, const int* gyroBefore, int kp, int kd);

#endif /* __MOTOR_H */
