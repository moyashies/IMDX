#ifndef __PD_H
#define __PD_H

extern int anglePd[2], gyroPd[3];

void setAnglePd(const int* gyro, const int* gyroBefore);
void setGyroPd(const int* gyro, const int* gyroBefore);

#endif /* __PD_H */
