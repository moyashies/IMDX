#include <math.h>

#include "../h/sensor.h"

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
