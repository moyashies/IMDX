#include "../h/pd.h"
#include "../h/receive.h"

void setAnglePd(const int* angle, const int* angleBefore)
{
    anglePd.x = angle[1] * (rx.buf[RX_ANGLE_KP] / 10)
        + (angleBefore[1] - angle[1]) * (rx.buf[RX_ANGLE_KD] / 10);
    anglePd.y = angle[0] * (rx.buf[RX_ANGLE_KP] / 10)
        + (angleBefore[0] - angle[0]) * (rx.buf[RX_ANGLE_KD] / 10);
}

void setGyroPd(const int* gyro, const int* gyroBefore)
{
    gyroPd.x = gyro[1] * (rx.buf[RX_ANGLE_KP] / 10)
        + (gyroBefore[1] - gyro[1]) * (rx.buf[RX_ANGLE_KD] / 10);
    gyroPd.y = gyro[0] * (rx.buf[RX_ANGLE_KP] / 10)
        + (gyroBefore[0] - gyro[0]) * (rx.buf[RX_ANGLE_KD] / 10);
}
