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

static inline int motor_();

void motorRefresh()
{
    PWML = motor.left;
    PWMR = motor.right / 2;
    PWMF = motor.front;
    PWMB = motor.back;
}

void motorSet()
{
    if (rx.buf[RX_MODE] & 0x10) {
        motor.left  = motor_( 0,  1, RX_MOTOR_L_BASE, PWM_L_BASE);
    } else {
        motor.left  = PWM_STOP;
    }

    if (rx.buf[RX_MODE] & 0x20) {
        motor.right = motor_( 0, -1, RX_MOTOR_R_BASE, PWM_R_BASE);
    } else {
        motor.right = PWM_STOP;
    }

    if (rx.buf[RX_MODE] & 0x40) {
        motor.front = motor_( 1,  0, RX_MOTOR_F_BASE, PWM_F_BASE);
    } else {
        motor.front = PWM_STOP;
    }

    if (rx.buf[RX_MODE] & 0x80) {
        motor.back  = motor_(-1,  0, RX_MOTOR_B_BASE, PWM_B_BASE);
    } else {
        motor.back  = PWM_STOP;
    }
}

void motorStop()
{
    motor.left  = PWM_STOP;
    motor.right = PWM_STOP;
    motor.front = PWM_STOP;
    motor.back  = PWM_STOP;
}

static inline int motor_(int x, int y, int base, int pwmBase)
{
    int pwm;
    int dimension, pos;

    /* mode: 1 1 1 1
             | | | |
             | | | '-- anglePd
             | | '---- gyroPd
             | '------ posture
             '-------- peak    */

    pwm = PWM_BASE + pwmBase + rx.buf[base]
        + (rx.buf[RX_THROTTLE] - 128) * PWM_THROTTLE;

    if (x != 0) {
        dimension = 0;
        pos = x;

        if (pos > 0) {
            pwm += (128 - rx.buf[RX_HANDLE_X]) * PWM_HANDLE;
        } else if (pos < 0) {
            pwm += (rx.buf[RX_HANDLE_X] - 128) * PWM_HANDLE;
        }

        if (rx.buf[RX_ROTATE] == 128) {
            if (rx.buf[RX_MODE] & 0x04) {
                pwm += gyro[2] * PWM_POSTURE;
            }
        } else {
            pwm += (128 - rx.buf[RX_ROTATE]) * PWM_ROTATE;
        }
    } else {
        dimension = 1;
        pos = y;

        if (pos > 0) {
            pwm += (rx.buf[RX_HANDLE_Y] - 128) * PWM_HANDLE;
        } else if (pos < 0) {
            pwm += (128 - rx.buf[RX_HANDLE_Y]) * PWM_HANDLE;
        }

        if (rx.buf[RX_ROTATE] == 128) {
            if (rx.buf[RX_MODE] & 0x04) {
                pwm += -gyro[2] * PWM_POSTURE;
            }
        } else {
            pwm += (rx.buf[RX_ROTATE] - 128) * PWM_ROTATE;
        }
    }

    if (angleBefore[dimension] > 0 && 0 >= angle[dimension]) {
        // leftが上がった状態から水平に向かうとき、オーバーしないようにする(オーバーした分戻す)
        if (rx.buf[RX_MODE] & 0x08) {
            pwm += pos * angle[dimension] * PWM_ANGLE_PEAK;
        }
    } else if (angleBefore[dimension] < 0 && 0 <= angle[dimension]) {
        if (rx.buf[RX_MODE] & 0x08) {
            pwm += -pos * angle[dimension] * PWM_ANGLE_PEAK;
        }
    } else {
        if (rx.buf[RX_MODE] & 0x01) {
            if (angle[dimension] < -20 && 20 < angle[dimension]) {
                pwm += pos * anglePd[dimension] * rx.buf[RX_ANGLE_GAIN];
            } else {
                if (angle[dimension] < -12 && 12 < angle[dimension]) {
                    pwm += pos * anglePd[dimension] * (rx.buf[RX_ANGLE_GAIN] * 2);
                } else {
                    pwm += pos * angle[dimension] * (rx.buf[RX_ANGLE_GAIN] / 2);
                }
            }
        }
        if (rx.buf[RX_MODE] & 0x02) {
            if (angle[dimension] < -8 && 8 < angle[dimension]) {
                pwm += (dimension == 0 ? pos : -pos)
                    * gyroPd[dimension] * rx.buf[RX_GYRO_GAIN];
            } else {
                pwm += (dimension == 0 ? pos : -pos)
                    * gyroPd[dimension] * (rx.buf[RX_GYRO_GAIN] / 2.0);
            }
        }
    }

    return PWM_LIMIT(pwm);
}
