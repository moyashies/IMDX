#include "../h/pd.h"
#include "../h/receive.h"
#include "../h/sensor.h"

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
