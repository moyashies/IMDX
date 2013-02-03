#include "../h/motor.h"
#include "../h/pd.h"
#include "../h/receive.h"

#define MIN(__a, __b) ((__a) < (__b) ? (__a) : (__b))
#define MAX(__a, __b) ((__a) > (__b) ? (__a) : (__b))
#define PWM_LIMIT(__pwm) MAX(PWM_MIN, MIN(__pwm, PWM_MAX));

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
            - anglePd[1] - gyroPd[1];

    if (rx.buf[RX_ROTATE] == 128) {
        pwm -= gyroZ * PWM_POSTURE;
    } else {
        pwm += (rx.buf[RX_ROTATE] - 128) * PWM_ROTATE;
    }

    motor.left = PWM_LIMIT(pwm);
}

void motorRight(int gyroZ)
{
    int pwm;

    pwm = PWM_R_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (128 - rx.buf[RX_HANDLE_Y]) * PWM_HANDLE
            + anglePd[1] + gyroPd[1];

    if (rx.buf[RX_ROTATE] == 128) {
        pwm -= gyroZ * PWM_POSTURE;
    } else {
        pwm += (rx.buf[RX_ROTATE] - 128) * PWM_ROTATE;
    }

    motor.right = PWM_LIMIT(pwm);
}

void motorFront(int gyroZ)
{
    int pwm;

    pwm = PWM_F_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (128 - rx.buf[RX_HANDLE_X]) * PWM_HANDLE
            - anglePd[0] + gyroPd[0];

    if (rx.buf[RX_ROTATE] == 128) {
        pwm += gyroZ * PWM_POSTURE;
    } else {
        pwm += (128 - rx.buf[RX_ROTATE]) * PWM_ROTATE;
    }

    motor.front = PWM_LIMIT(pwm);
}

void motorBack(int gyroZ)
{
    int pwm;

    pwm = PWM_B_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (rx.buf[RX_HANDLE_X] - 128) * PWM_HANDLE
            + anglePd[0] - gyroPd[0];

    if (rx.buf[RX_ROTATE] == 128) {
        pwm += gyroZ * PWM_POSTURE;
    } else {
        pwm += (128 - rx.buf[RX_ROTATE]) * PWM_ROTATE;
    }

    motor.back = PWM_LIMIT(pwm);
}
