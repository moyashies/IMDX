#ifndef __MOTOR_H
#define __MOTOR_H

#define PWM_BASE 21000
#define PWM_L_BASE 20930
#define PWM_R_BASE 20870
#define PWM_F_BASE PWM_BASE
#define PWM_B_BASE 20900

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

void motorSet();
void motorStop();

#endif /* __MOTOR_H */
