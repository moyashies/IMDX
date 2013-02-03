#include <math.h>

#include "../h/sensor.h"

/* taken from math.h */
#define M_PI 3.14159265358979323846264338327950288

#define SENSOR_CYCLE 0.2
#define ANGULAR_VELOCITY 0.6283185307179586 /* 2 * M_PI * 0.1 */
#define ANGLE 0.12566370614359174 /* ANGULAR_VELOCITY * SENSOR_CYCLE */

void setAngleX()
{
    angle[0] -= angle[0] * ANGLE + angleAcceX() * ANGLE
        + gyro[0] * SENSOR_CYCLE;
}

void setAngleY()
{
    angle[1] -= angle[1] * ANGLE + angleAcceY() * ANGLE
        + gyro[1] * SENSOR_CYCLE;
}

int angleAcceX(const int* acce)
{
    return (int)(atan2f((float)acce[1], (float)-acce[2]) / M_PI * 180);
}

int angleAcceY(const int* acce)
{
    return (int)(atan2f((float)acce[0], (float)-acce[2]) / M_PI * 180);
}

int angleAcceZ(const int* acce)
{
    return (int)(atan2f((float)acce[0], (float)-acce[1]) / M_PI * 180);
}
