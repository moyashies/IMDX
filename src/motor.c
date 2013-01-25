#include "../h/motor.h"
#include "../h/receive.h"

#define MIN(__a, __b) ((__a) < (__b) ? (__a) : (__b))
#define MAX(__a, __b) ((__a) > (__b) ? (__a) : (__b))

static inline int pwm_limit(int pwm);

void motorStop()
{
    motor.left  = PWM_STOP;
    motor.right = PWM_STOP;
    motor.front = PWM_STOP;
    motor.back  = PWM_STOP;
}

void motorLeft(int gyroZ, int angleXPD, int gyroXPD)
{
    int pwm;

    pwm = PWM_L_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (rx.buf[RX_HANDLE_Y] - 128) * PWM_HANDLE
            - angleXPD - gyroXPD;

    if (rx.buf[RX_ROTATE] == 128) {
        pwm -= gyroZ * PWM_POSTURE;
    } else {
        pwm += (rx.buf[RX_ROTATE] - 128) * PWM_ROTATE;
    }

    motor.left = pwm_limit(pwm);
}

void motorRight(int gyroZ, int angleXPD, int gyroXPD)
{
    int pwm;

    pwm = PWM_R_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (128 - rx.buf[RX_HANDLE_Y]) * PWM_HANDLE
            + angleXPD + gyroXPD;

    if (rx.buf[RX_ROTATE] == 128) {
        pwm -= gyroZ * PWM_POSTURE;
    } else {
        pwm += (rx.buf[RX_ROTATE] - 128) * PWM_ROTATE;
    }

    motor.right = pwm_limit(pwm);
}

void motorFront(int gyroZ, int angleYPD, int gyroYPD)
{
    int pwm;

    pwm = PWM_F_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (128 - rx.buf[RX_HANDLE_X]) * PWM_HANDLE
            - angleYPD + gyroYPD;

    if (rx.buf[RX_ROTATE] == 128) {
        pwm += gyroZ * PWM_POSTURE;
    } else {
        pwm += (128 - rx.buf[RX_ROTATE]) * PWM_ROTATE;
    }

    motor.front = pwm_limit(pwm);
}

void motorBack(int gyroZ, int angleYPD, int gyroYPD)
{
    int pwm;

    pwm = PWM_B_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (rx.buf[RX_HANDLE_X] - 128) * PWM_HANDLE
            + angleYPD - gyroYPD;

    if (rx.buf[RX_ROTATE] == 128) {
        pwm += gyroZ * PWM_POSTURE;
    } else {
        pwm += (128 - rx.buf[RX_ROTATE]) * PWM_ROTATE;
    }

    motor.back = pwm_limit(pwm);
}

static inline int pwm_limit(int pwm)
{
    return MAX(PWM_MIN, MIN(pwm, PWM_MAX));
}

int _my_angleXPD(const int* angle, const int* angleBefore, int kp, int kd)
{
    return angle[1] * (kp / 10)
        + (angleBefore[1] - angle[1]) * (kd / 10);
}

int _my_angleYPD(const int* angle, const int* angleBefore, int kp, int kd)
{
    return angle[0] * (kp / 10)
        + (angleBefore[0] - angle[0]) * (kd / 10);
}

int _my_gyroXPD(const int* gyro, const int* gyroBefore, int kp, int kd)
{
    return gyro[1] * (kp / 10)
        + (gyroBefore[1] - gyro[1]) * (kd / 10);
}

int _my_gyroYPD(const int* gyro, const int* gyroBefore, int kp, int kd)
{
    return gyro[0] * (kp / 10)
        + (gyroBefore[0] - gyro[0]) * (kd / 10);
}
