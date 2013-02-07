#ifndef __SENSOR_H
#define __SENSOR_H

extern int acce[3], gyro[3];
extern int angle[2];
extern int angleBefore[2], gyroBefore[3];
extern int anglePd[2], gyroPd[3];

void setAngle();

void setAnglePd();
void setGyroPd();

#endif /* __SENSOR_H */
