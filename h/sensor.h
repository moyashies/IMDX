#ifndef __SENSOR_H
#define __SENSOR_H

extern int acce[3], gyro[3];
extern int angle[2];
extern int angleBefore[2], gyroBefore[3];

void setAngleX();
void setAngleY();

int angleAcceX();
int angleAcceY();
int angleAcceZ();

#endif /* __SENSOR_H */
