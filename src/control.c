#include <math.h>

#include "../h/control.h"

#define LIMIT_PWM_MIN(__pwm) (__pwm > PWM_MIN ? __pwm : PWM_MIN)

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

int _my_angleXPD(const int* angle, const int* angleBefore)
{
    return angle[1] * ACCE_KP
        + (angleBefore[1] - angle[1]) * ACCE_KD;
}

int _my_angleYPD(const int* angle, const int* angleBefore)
{
    return angle[0] * ACCE_KP
        + (angleBefore[0] - angle[0]) * ACCE_KD;
}

int _my_gyroXPD(const int* gyro, const int* gyroBefore)
{
    return gyro[1] * GYRO_KP
        + (gyroBefore[1] - gyro[1]) * GYRO_KD;
}

int _my_gyroYPD(const int* gyro, const int* gyroBefore)
{
    return gyro[0] * GYRO_KP
        + (gyroBefore[0] - gyro[0]) * GYRO_KD;
}

unsigned int _my_pwml(const unsigned int* received, const int* gyro,
        int angleXPD, int gyroXPD)
{
    return LIMIT_PWM_MIN(
        PWM_MIN
        + (received[4] - 128) * PWM_THROTTLE        // speed
        + (received[3] - 128) * PWM_HANDLE          // move_x
            - angleXPD - gyroXPD - gyro[2] * 16
    );
}

unsigned int _my_pwmr(const unsigned int* received, const int* gyro,
        int angleXPD, int gyroXPD)
{
    return LIMIT_PWM_MIN(
        PWM_MIN
        + (received[4] - 128) * PWM_THROTTLE        // speed
        + (128 - received[3]) * PWM_HANDLE          // move_x
            + angleXPD + gyroXPD - gyro[2] * 16
    );
}

unsigned int _my_pwmf(const unsigned int* received, const int* gyro,
        int angleXPD, int gyroXPD)
{
    return LIMIT_PWM_MIN(
        PWM_MIN
        + (received[4] - 128) * PWM_THROTTLE        // speed
        + (128 - received[2]) * PWM_HANDLE          // move_y
            - angleXPD + gyroXPD + gyro[2] * 16
    );
}

unsigned int _my_pwmb(const unsigned int* received, const int* gyro,
        int angleXPD, int gyroXPD)
{
    return LIMIT_PWM_MIN(
        PWM_MIN
        + (received[4] - 128) * PWM_THROTTLE        // speed
        + (received[2] - 128) * PWM_HANDLE          // move_y
            + angleXPD - gyroXPD + gyro[2] * 16
    );
}
