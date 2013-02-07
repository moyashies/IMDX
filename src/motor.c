#ifndef TEST_IMDX
#  include <xc.h>
#endif
#include "../h/motor.h"
#include "../h/receive.h"
#include "../h/sensor.h"

#define PWML PDC3
#define PWMR OC2R /* differ from other modules, you have to divide by 2. */
#define PWMF PDC1
#define PWMB PDC2

#define MIN(__a, __b) ((__a) < (__b) ? (__a) : (__b))
#define MAX(__a, __b) ((__a) > (__b) ? (__a) : (__b))
#define PWM_LIMIT(__pwm) MAX(PWM_MIN, MIN(__pwm, PWM_MAX))

static inline void motorLeft();
static inline void motorRight();
static inline void motorFront();
static inline void motorBack();

void motorRefresh()
{
    PWML = motor.left;
    PWMR = motor.right / 2;
    PWMF = motor.front;
    PWMB = motor.back;
}

void motorSet()
{
    motorLeft();
    motorRight();
    motorFront();
    motorBack();
}

void motorStop()
{
    motor.left  = PWM_STOP;
    motor.right = PWM_STOP;
    motor.front = PWM_STOP;
    motor.back  = PWM_STOP;
}

/* up: -angle, +gyro */
static inline void motorLeft()
{
    int pwm;

    pwm = PWM_L_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (rx.buf[RX_HANDLE_Y] - 128) * PWM_HANDLE;

    if (-30 < gyroPd[1] && gyroPd[1] < 30) {
        pwm += anglePd[1] * rx.buf[RX_ANGLE_GAIN];
    }
    pwm -= gyroPd[1] * rx.buf[RX_GYRO_GAIN];

    if (rx.buf[RX_ROTATE] == 128) {
        pwm -= gyroPd[2] * PWM_POSTURE;
    } else {
        pwm += (rx.buf[RX_ROTATE] - 128) * PWM_ROTATE;
    }

    motor.left = PWM_LIMIT(pwm);
}

/* up: +angle, -gyro */
static inline void motorRight()
{
    int pwm;

    pwm = PWM_R_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (128 - rx.buf[RX_HANDLE_Y]) * PWM_HANDLE;

    if (-30 < gyroPd[1] && gyroPd[1] < 30) {
        pwm -= anglePd[1] * rx.buf[RX_ANGLE_GAIN];
    }
    pwm += gyroPd[1] * rx.buf[RX_GYRO_GAIN];

    if (rx.buf[RX_ROTATE] == 128) {
        pwm -= gyroPd[2] * PWM_POSTURE;
    } else {
        pwm += (rx.buf[RX_ROTATE] - 128) * PWM_ROTATE;
    }

    motor.right = PWM_LIMIT(pwm);
}

/* up: -angle, -gyro */
static inline void motorFront()
{
    int pwm;

    pwm = PWM_F_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (128 - rx.buf[RX_HANDLE_X]) * PWM_HANDLE;

    if (-30 < gyroPd[0] && gyroPd[0] < 30) {
        pwm += anglePd[0] * rx.buf[RX_ANGLE_GAIN];
    }
    pwm += gyroPd[0] * rx.buf[RX_GYRO_GAIN];

    if (rx.buf[RX_ROTATE] == 128) {
        pwm += gyroPd[2] * PWM_POSTURE;
    } else {
        pwm += (128 - rx.buf[RX_ROTATE]) * PWM_ROTATE;
    }

    motor.front = PWM_LIMIT(pwm);
}

/* up: +angle, +gyro */
static inline void motorBack()
{
    int pwm;

    pwm = PWM_B_BASE
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE
        + (rx.buf[RX_HANDLE_X] - 128) * PWM_HANDLE;

    if (-30 < gyroPd[0] && gyroPd[0] < 30) {
        pwm -= anglePd[0] * rx.buf[RX_ANGLE_GAIN];
    }
    pwm -= gyroPd[0] * rx.buf[RX_GYRO_GAIN];

    if (rx.buf[RX_ROTATE] == 128) {
        pwm += gyroPd[2] * PWM_POSTURE;
    } else {
        pwm += (128 - rx.buf[RX_ROTATE]) * PWM_ROTATE;
    }

    motor.back = PWM_LIMIT(pwm);
}
