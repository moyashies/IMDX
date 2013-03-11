#ifndef __MOTOR_H
#define __MOTOR_H

#define PWM_BASE 20000

#define PWM_L_BASE 1000
#define PWM_R_BASE 800
#define PWM_F_BASE 800
#define PWM_B_BASE 700

#define PWM_MAX 28000
#define PWM_MIN 20000
#define PWM_STOP 18000

#define PWM_THROTTLE 16
#define PWM_HANDLE 1
#define PWM_POSTURE 12
#define PWM_ROTATE 4

#define PWM_ANGLE_PEAK 8

struct pwm {
    int left;
    int right;
    int front;
    int back;
};

extern struct pwm motor;

void motorRefresh();
void motorSet();
void motorStop();

#endif /* __MOTOR_H */
