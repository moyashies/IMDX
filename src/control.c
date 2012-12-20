#include <math.h>

#include "../h/control.h"

#define LIMIT_PWM(__pwm) (__pwm > PWM_MIN_LIMIT ? __pwm : PWM_MIN_LIMIT)

int angleX(const int* acce)
{
    return (int)(atan2f((float)acce[1], (float)-acce[2]) / M_PI * 180);
}

int angleY(const int* acce)
{
    return (int)(atan2f((float)acce[0], (float)-acce[2]) / M_PI * 180);
}

int angleZ(const int* acce)
{
    return (int)(atan2f((float)acce[0], (float)-acce[1]) / M_PI * 180);
}

int _my_angleXPD(const int* angle, const int* angleBefore, int kp, int kd)
{
    return angle[1] * kp
        + (angleBefore[1] - angle[1]) * kd;
}

int _my_angleYPD(const int* angle, const int* angleBefore, int kp, int kd)
{
    return angle[0] * kp
        + (angleBefore[0] - angle[0]) * kd;
}

int _my_gyroXPD(const int* gyro, const int* gyroBefore, int kp, int kd)
{
    return gyro[1] * kp
        + (gyroBefore[1] - gyro[1]) * kd;
}

int _my_gyroYPD(const int* gyro, const int* gyroBefore, int kp, int kd)
{
    return gyro[0] * kp
        + (gyroBefore[0] - gyro[0]) * kd;
}

unsigned int _my_pwml(const unsigned int* received, const int* gyro,
        int angleXPD, int gyroXPD)
{
    return LIMIT_PWM(
        PWM_MIN
        + (received[4] - 128) * PWM_THROTTLE        // speed
        + (received[3] - 128) * PWM_HANDLE          // move_x
            - angleXPD - gyroXPD - gyro[2] * 16
    );
}

unsigned int _my_pwmr(const unsigned int* received, const int* gyro,
        int angleXPD, int gyroXPD)
{
    return LIMIT_PWM(
        PWM_MIN
        + (received[4] - 128) * PWM_THROTTLE        // speed
        + (128 - received[3]) * PWM_HANDLE          // move_x
            + angleXPD + gyroXPD - gyro[2] * 16
    );
}

unsigned int _my_pwmf(const unsigned int* received, const int* gyro,
        int angleYPD, int gyroYPD)
{
    return LIMIT_PWM(
        PWM_MIN
        + (received[4] - 128) * PWM_THROTTLE        // speed
        + (128 - received[2]) * PWM_HANDLE          // move_y
            - angleYPD + gyroYPD + gyro[2] * 16
    );
}

unsigned int _my_pwmb(const unsigned int* received, const int* gyro,
        int angleYPD, int gyroYPD)
{
    return LIMIT_PWM(
        PWM_MIN
        + (received[4] - 128) * PWM_THROTTLE        // speed
        + (received[2] - 128) * PWM_HANDLE          // move_y
            + angleYPD - gyroYPD + gyro[2] * 16
    );
}
