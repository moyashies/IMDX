#ifndef __PD_H
#define __PD_H

struct pd {
    int x;
    int y;
};

extern struct pd anglePd, gyroPd;

void setAnglePd(const int* gyro, const int* gyroBefore);
void setGyroPd(const int* gyro, const int* gyroBefore);

#endif /* __PD_H */
