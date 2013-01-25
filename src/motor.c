#include "../h/motor.h"
#include "../h/pd.h"
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

void motorLeft(int gyroZ)
{
    int pwm;

    pwm = PWM_L_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (rx.buf[RX_HANDLE_Y] - 128) * PWM_HANDLE
            - anglePd.x - gyroPd.x;

    if (rx.buf[RX_ROTATE] == 128) {
        pwm -= gyroZ * PWM_POSTURE;
    } else {
        pwm += (rx.buf[RX_ROTATE] - 128) * PWM_ROTATE;
    }

    motor.left = pwm_limit(pwm);
}

void motorRight(int gyroZ)
{
    int pwm;

    pwm = PWM_R_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (128 - rx.buf[RX_HANDLE_Y]) * PWM_HANDLE
            + anglePd.x + gyroPd.x;

    if (rx.buf[RX_ROTATE] == 128) {
        pwm -= gyroZ * PWM_POSTURE;
    } else {
        pwm += (rx.buf[RX_ROTATE] - 128) * PWM_ROTATE;
    }

    motor.right = pwm_limit(pwm);
}

void motorFront(int gyroZ)
{
    int pwm;

    pwm = PWM_F_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (128 - rx.buf[RX_HANDLE_X]) * PWM_HANDLE
            - anglePd.y + gyroPd.y;

    if (rx.buf[RX_ROTATE] == 128) {
        pwm += gyroZ * PWM_POSTURE;
    } else {
        pwm += (128 - rx.buf[RX_ROTATE]) * PWM_ROTATE;
    }

    motor.front = pwm_limit(pwm);
}

void motorBack(int gyroZ)
{
    int pwm;

    pwm = PWM_B_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (rx.buf[RX_HANDLE_X] - 128) * PWM_HANDLE
            + anglePd.y - gyroPd.y;

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
