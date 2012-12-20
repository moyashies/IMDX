#ifndef __CONTROL_H
#define __CONTROL_H

/* taken from math.h */
#define M_PI 3.14159265358979323846264338327950288

#define PWM_MAX 30000
#define PWML_MAX 30000
#define PWMR_MAX 15000
#define PWMF_MAX 30000
#define PWMB_MAX 30000

#define PWM_MIN 21000
#define PWML_MIN 21200
#define PWMR_MIN 10600
#define PWMF_MIN 21200
#define PWMB_MIN 21200

#define PWM_MIN_LIMIT 20000

#define PWM_STOP 18000
#define PWML_STOP 18000
#define PWMR_STOP 9000
#define PWMF_STOP 18000
#define PWMB_STOP 18000

#define PWM_THROTTLE 8
#define PWML_THROTTLE 8
#define PWMR_THROTTLE 4
#define PWMF_THROTTLE 8
#define PWMB_THROTTLE 8

#define PWM_HANDLE 2
#define PWML_HANDLE 2
#define PWMR_HANDLE 1
#define PWMF_HANDLE 2
#define PWMB_HANDLE 2

#define ACCE_KP 6
#define ACCE_KD 30

#define GYRO_KP 5
#define GYRO_KD 65

int angleX(const int* acce);
int angleY(const int* acce);
int angleZ(const int* acce);

unsigned int _my_pwml(const unsigned int* received, const int* gyro, int angleX, int gyroX);
unsigned int _my_pwmr(const unsigned int* received, const int* gyro, int angleX, int gyroX);
unsigned int _my_pwmf(const unsigned int* received, const int* gyro, int angleX, int gyroX);
unsigned int _my_pwmb(const unsigned int* received, const int* gyro, int angleX, int gyroX);

int _my_angleXPD(const int* angle, const int* angleBefore, int kp, int kd);
int _my_angleYPD(const int* angle, const int* angleBefore, int kp, int kd);
int _my_gyroXPD(const int* gyro, const int* gyroBefore, int kp, int kd);
int _my_gyroYPD(const int* gyro, const int* gyroBefore, int kp, int kd);

#endif /* __CONTROL_H */
