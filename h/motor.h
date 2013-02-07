#ifndef __MOTOR_H
#define __MOTOR_H

#define PWM_BASE 20000

#define PWM_MAX 28000
#define PWM_MIN 20000
#define PWM_STOP 18000

#define PWM_THROTTLE 14
#define PWM_HANDLE 2
#define PWM_POSTURE 14
#define PWM_ROTATE 4

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
