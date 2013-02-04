#include <math.h>

#include "../h/receive.h"
#include "../h/sensor.h"

/* taken from math.h */
#define M_PI 3.14159265358979323846264338327950288

#define SENSOR_CYCLE 0.2
#define ANGULAR_VELOCITY 0.6283185307179586 /* 2 * M_PI * 0.1 */
#define ANGLE 0.12566370614359174 /* ANGULAR_VELOCITY * SENSOR_CYCLE */

static inline int angleAcceX();
static inline int angleAcceY();
static inline int angleAcceZ();

/* roll angle */
void setAngleX()
{
    angle[0] -= angle[0] * ANGLE + angleAcceX() * ANGLE
        + gyro[0] * SENSOR_CYCLE;
}

/* pitch angle */
void setAngleY()
{
    angle[1] -= angle[1] * ANGLE + angleAcceY() * ANGLE
        + gyro[1] * SENSOR_CYCLE;
}

void setAnglePd()
{
    anglePd[0] = angle[0] * (rx.buf[RX_ANGLE_KP] / 10)
        + (angleBefore[0] - angle[0]) * (rx.buf[RX_ANGLE_KD] / 10);
    anglePd[1] = angle[1] * (rx.buf[RX_ANGLE_KP] / 10)
        + (angleBefore[1] - angle[1]) * (rx.buf[RX_ANGLE_KD] / 10);
}

void setGyroPd()
{
    gyroPd[0] = gyro[0] * (rx.buf[RX_ANGLE_KP] / 10)
        + (gyroBefore[0] - gyro[0]) * (rx.buf[RX_ANGLE_KD] / 10);
    gyroPd[1] = gyro[1] * (rx.buf[RX_ANGLE_KP] / 10)
        + (gyroBefore[1] - gyro[1]) * (rx.buf[RX_ANGLE_KD] / 10);
    gyroPd[2] = gyro[2] * (rx.buf[RX_ANGLE_KP] / 10)
        + (gyroBefore[2] - gyro[2]) * (rx.buf[RX_ANGLE_KD] / 10);
}

static inline int angleAcceX()
{
    return (int)(atan2f((float)acce[1], (float)-acce[2]) / M_PI * 180);
}

static inline int angleAcceY()
{
    return (int)(atan2f((float)acce[0], (float)-acce[2]) / M_PI * 180);
}

static inline int angleAcceZ()
{
    return (int)(atan2f((float)acce[0], (float)-acce[1]) / M_PI * 180);
}
