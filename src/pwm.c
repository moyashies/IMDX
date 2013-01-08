#include "../h/pwm.h"

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

unsigned int PWMLeft(const unsigned int* received, int gyroZ,
        int angleXPD, int gyroXPD)
{
    int pwm =
        PWM_L_BASE
        + (received[4] - 128) * PWM_THROTTLE
        + (received[3] - 128) * PWM_HANDLE
            - angleXPD - gyroXPD;

    if (received[1] == 128) {
        pwm -= gyroZ * PWM_POSTURE;
    } else {
        pwm += (received[1] - 128) * PWM_ROTATE;
    }

    return MAX(PWM_MIN, MIN(pwm, PWM_MAX));
}

unsigned int PWMRight(const unsigned int* received, int gyroZ,
        int angleXPD, int gyroXPD)
{
    int pwm =
        PWM_R_BASE
        + (received[4] - 128) * PWM_THROTTLE
        + (128 - received[3]) * PWM_HANDLE
            + angleXPD + gyroXPD;

    if (received[1] == 128) {
        pwm -= gyroZ * PWM_POSTURE;
    } else {
        pwm += (received[1] - 128) * PWM_ROTATE;
    }

    return MAX(PWM_MIN, MIN(pwm, PWM_MAX));
}

unsigned int PWMFront(const unsigned int* received, int gyroZ,
        int angleYPD, int gyroYPD)
{
    int pwm =
        PWM_F_BASE
        + (received[4] - 128) * PWM_THROTTLE
        + (128 - received[2]) * PWM_HANDLE
            - angleYPD + gyroYPD;

    if (received[1] == 128) {
        pwm += gyroZ * PWM_POSTURE;
    } else {
        pwm += (128 - received[1]) * PWM_ROTATE;
    }

    return MAX(PWM_MIN, MIN(pwm, PWM_MAX));
}

unsigned int PWMBack(const unsigned int* received, int gyroZ,
        int angleYPD, int gyroYPD)
{
    int pwm =
        PWM_B_BASE
        + (received[4] - 128) * PWM_THROTTLE
        + (received[2] - 128) * PWM_HANDLE
            + angleYPD - gyroYPD;

    if (received[1] == 128) {
        pwm += gyroZ * PWM_POSTURE;
    } else {
        pwm += (128 - received[1]) * PWM_ROTATE;
    }

    return MAX(PWM_MIN, MIN(pwm, PWM_MAX));
}
