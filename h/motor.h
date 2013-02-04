#ifndef __MOTOR_H
#define __MOTOR_H

#define PWM_BASE 21000
#define PWM_L_BASE 20930
#define PWM_R_BASE 20870
#define PWM_F_BASE 21060
#define PWM_B_BASE 21060

#define PWM_MAX 28000
#define PWM_MIN 20000
#define PWM_STOP 18000

#define PWM_THROTTLE 14
#define PWM_HANDLE 1
#define PWM_POSTURE 12
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

void motorSet();
void motorStop();

#endif /* __MOTOR_H */
