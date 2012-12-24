#include <string.h>
#include <xc.h>
#include "../h/i2cEmem.h"

#include "../h/protocol.h"
#include "../h/pwm.h"
#include "../h/sensor.h"
#include "../h/st.h"

_FICD(ICS_PGD3 & JTAGEN_OFF);
_FPOR(ALTI2C1_OFF & ALTI2C2_ON & WDTWIN_WIN50);
_FWDT(WDTPOST_PS32768 & WDTPRE_PR128 & PLLKEN_ON & WINDIS_OFF &
      FWDTEN_OFF);
_FOSC(POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSDCMD);
_FOSCSEL(FNOSC_FRCPLL & PWMLOCK_OFF & IESO_OFF);
_FGS(GWRP_OFF & GCP_OFF);

#define P_PSW (~PORTB & 0x0080)
#define P_SSW (~PORTB & 0x0100)
#define P_BEEP LATBbits.LATB9
#define P_LED0 LATAbits.LATA2
#define P_LED1 LATAbits.LATA3
#define P_LED2 LATBbits.LATB0
#define P_LED3 LATBbits.LATB1

#define PWML PDC3
#define PWMR OC2R /* differ from other modules, you have to divide by 2. */
#define PWMF PDC1
#define PWMB PDC2

void initOsc();
void initPort();
void initPmd();
void initPwm();
void initAd();
void initUART();
void initT1();
void initT3();
void initOC1();
void initInt();
void stopMotor();
void beep(unsigned int wait);

unsigned char getChar(void);
void setChar(unsigned char c);

// Instantiate Drive and Data objects
I2CEMEM_DRV i2cmem = I2CSEMEM_DRV_DEFAULTS;
I2CEMEM_DATA wData;
I2CEMEM_DATA rData;

unsigned int wBuff[10], rBuff[10];

unsigned int i = 0;

unsigned int received[15];
unsigned int dataOK = 0;

unsigned int pswC = 0;
unsigned int pswF = 0;
unsigned int pswV = 0;

unsigned int sswC = 0;
unsigned int sswF = 0;
unsigned int sswV = 0;

unsigned int leds = 0;
unsigned int ledCount = 0;

unsigned int motorEnable = 0;

unsigned int receivedNum = 0;
unsigned char isTimeout = 1;

int angle[3], acce[3], gyro[3];

int angleBefore[3] = {};
int acceBefore[3] = {};
int gyroBefore[3] = {};

int angleXPD, angleYPD, gyroXPD, gyroYPD;

unsigned int pwml, pwmr, pwmf, pwmb;

unsigned char bufNum = 0;
unsigned char bufIndex = 0;
unsigned int buf[128];

unsigned char getChar()
{
    unsigned char c;
    c = buf[bufIndex % 128];
    bufIndex++;
    bufNum--;
    return c;
}

void setChar(unsigned char c)
{
    while (num == 128);

    buf[(bufIndex + bufNum) % 128] = c;
    num++;
    if (!IEC0bits.U1TXIE) {
        U1TXREG = getChar();
        IFS0bits.U1TXIF = 0;
        IEC0bits.U1TXIE = 1;
    }
}

int main(void)
{
    initOsc();
    initPort();
    initPwm();
    initAd();
    initUART();
    initT1();
    initT3();
    initOC1();
    initInt();
    stopMotor();

    i2cmem.init(&i2cmem);

    // Initialise Data to be written to serial EEPROM
    for (i = 0; i < 10; i++)
        wBuff[i] = i;

    // Initialise I2C Data object for Write operation
    wData.buff = wBuff;
    wData.n = 1;
    wData.addr = 0x28;
    wData.devSel = 0x00;

    // Initialise I2C Data Object for Read operation
    rData.buff = rBuff;
    rData.n = 1;
    rData.addr = 0x29;
    rData.devSel = 0x00;

    INFOF("starting...");
    leds = 0x0f;
    beep(500);
    stopMotor();

    // Initialise ACCE
    wData.devSel = ACCE;
    wData.addr = 0x20;
    wData.n = 1;

    wBuff[0] = 0x77;
    // Write Data
    i2cmem.oData = &wData;
    i2cmem.cmd = I2C_WRITE;

    while (i2cmem.cmd != I2C_IDLE) {
        i2cmem.tick(&i2cmem);
    }

    // Initialise GYRO
    wData.devSel = GYRO;
    wData.addr = 0x20;
    wData.n = 1;

    wBuff[0] = 0x0f;
    // Write Data
    i2cmem.oData = &wData;
    i2cmem.cmd = I2C_WRITE;

    while (i2cmem.cmd != I2C_IDLE) {
        i2cmem.tick(&i2cmem);
    }

    wData.devSel = GYRO;
    wData.addr = 0x21;
    wData.n = 1;

    wBuff[0] = 0x09;
    // Write Data
    i2cmem.oData = &wData;
    i2cmem.cmd = I2C_WRITE;

    while (i2cmem.cmd != I2C_IDLE) {
        i2cmem.tick(&i2cmem);
    }

    wData.devSel = GYRO;
    wData.addr = 0x23;
    wData.n = 1;

    wBuff[0] = 0x00;
    // Write Data
    i2cmem.oData = &wData;
    i2cmem.cmd = I2C_WRITE;

    while (i2cmem.cmd != I2C_IDLE) {
        i2cmem.tick(&i2cmem);
    }

    INFOF("started!");
    while (1) {
        if (isTimeout) {
            leds = 0x0f;
            continue;
        }

        motorEnable = sswV ? 1 : 0;
        if (pswF && pswV) {
            pswF = 0;
            INFOF("s");
        }
        leds ^= 0x04;
        if (dataOK) {
            dataOK = 0;
            leds ^= 0x08;
            leds = received[0];
        }

        // Read Data
        rData.devSel = ACCE;
        rData.addr = 0xa9;
        rData.n = 5;
        i2cmem.oData = &rData;
        i2cmem.cmd = I2C_READ;
        while (i2cmem.cmd != I2C_IDLE) {
            i2cmem.tick(&i2cmem);
        }

        acce[0] = (char)rBuff[0];
        acce[1] = (char)rBuff[2];
        acce[2] = (char)rBuff[4];

        acce[0] = (int)(0.95 * acceBefore[0] + 0.05 * acce[0]);
        acce[1] = (int)(0.95 * acceBefore[1] + 0.05 * acce[1]);
        acce[2] = (int)(0.95 * acceBefore[2] + 0.05 * acce[2]);
        memcpy(acceBefore, acce, sizeof(acceBefore));

        // Read Data
        rData.devSel = GYRO;
        rData.addr = 0xa9;
        rData.n = 5;
        i2cmem.oData = &rData;
        i2cmem.cmd = I2C_READ;
        while (i2cmem.cmd != I2C_IDLE) {
            i2cmem.tick(&i2cmem);
        }

        gyro[0] = (char)rBuff[0];
        gyro[1] = (char)rBuff[2];
        gyro[2] = (char)rBuff[4];
        memcpy(gyroBefore, gyro, sizeof(gyroBefore));

        angle[0] = angleAcceX(acce);
        angle[1] = angleAcceY(acce);
        angle[2] = angleAcceZ(acce);
        memcpy(angleBefore, angle, sizeof(angleBefore));

        stf("A,%d,%d,%d,"
            "%d,%d,%d,"
            "%d,%d,%d\n",
            acce[0], acce[1], acce[2],
            gyro[0], gyro[1], gyro[2],
            angle[0], angle[1], angle[2]);

        stf("M,%d,%d,%d,%d\n",
            pwml, pwmr, pwmf, pwmb);
    }
    return 0;
}

void _ISR _DefaultInterrupt(void)
{
    asm volatile ("RESET");
}

void _ISR _U1RXInterrupt(void)
{
    unsigned int c;

    IFS0bits.U1RXIF = 0;
    c = U1RXREG;

    receivedNum++;

    leds = c & 0x0f;

    switch (c) {
    case 'q':
        clearReceivingBuffer();
        break;
    case 'r':
        if (doneReceiving(received)) {
            dataOK = 1;
        }
        break;
    case 'a':
    case 'b': /* rotate */
    case 'c': /* handle x */
    case 'd': /* handle y */
    case 'e': /* throttle */
    case 'f': /* enable switch */
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
        prepareReceive(c);
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        receiveNum(c);
        break;
    }
}

void _ISR _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;

    if (bufNum > 0) {
        U1TXREG = getChar();
    } else {
        IEC0bits.U1TXIE = 0;
    }
}

void _ISRFAST _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    ClrWdt();

    ledCount++;
    if (ledCount % 16) {
        P_LED0 = 0;
        P_LED1 = 0;
    } else {
        if (leds & 1) P_LED0 = 1;
        if (leds & 2) P_LED1 = 1;
    }
    if (ledCount % 2) {
        P_LED2 = 0;
        P_LED3 = 0;
    } else {
        if (leds & 4) P_LED2 = 1;
        if (leds & 8) P_LED3 = 1;
    }

    if (i < 65535)
        i++;

    if (P_PSW != pswV) {
        if (++pswC > 50) {
            pswV = P_PSW ? 1 : 0;
            pswF = 1;
            pswC = 0;
        }
    } else {
        pswC = 0;
    }

    if (P_SSW != sswV) {
        if (++sswC > 50) {
            sswV = P_SSW ? 1 : 0;
            sswF = 1;
            sswC = 0;
        }
    } else {
        sswC = 0;
    }

    // バランス制御をここに入れる
    // 前に傾いている => 前の出力UP
    // 左に傾いている => 左の出力UP
    // 時計回りで自転 => 時計回りのローターの出力UP
    // 時計回りで自転 => 時計回りのローターの出力UP
    // 高度をできるだけ保つ

    if (motorEnable && !isTimeout) {
        if (dataOK) {
            dataOK = 0;
            if (received[5] == 0) {
                pwml = PWM_STOP;
                pwmr = PWM_STOP;
                pwmf = PWM_STOP;
                pwmb = PWM_STOP;
            } else {
                angleXPD = _my_angleXPD(angle, angleBefore,
                        received[7] / 4, received[8] / 4);
                angleYPD = _my_angleYPD(angle, angleBefore,
                        received[7] / 4, received[8] / 4);

                gyroXPD = _my_gyroXPD(gyro, gyroBefore,
                        received[9] / 4, received[10] / 4);
                gyroYPD = _my_gyroYPD(gyro, gyroBefore,
                        received[9] / 4, received[10] / 4);

                pwml = PWMLeft(received, gyro, angleXPD, gyroXPD);
                pwmr = PWMRight(received, gyro, angleXPD, gyroXPD);
                pwmf = PWMFront(received, gyro, angleYPD, gyroYPD);
                pwmb = PWMBack(received, gyro, angleYPD, gyroYPD);
            }
        }
    } else {
        pwml = PWM_STOP;
        pwmr = PWM_STOP;
        pwmf = PWM_STOP;
        pwmb = PWM_STOP;
    }

    PWML = pwml;
    PWMR = pwmr / 2;
    PWMF = pwmf;
    PWMB = pwmb;
}

void _ISRFAST _T3Interrupt(void)
{
    IFS0bits.T3IF = 0;

    isTimeout = receivedNum < 1 ? 1 : 0;
    receivedNum = 0;
}

void initOsc()
{
    // FOSC = 140MHz
    // PLLPRE = 0
    // PLLPOST = 0
    // PLLDIV = 73
    CLKDIV = 0x0000;
    PLLFBD = 73;
    OSCTUN = 0;
    REFOCON = 0x8000;
    OSCCON = 0x0100;
    OSCCON |= 0x0001;
    while (OSCCONbits.LOCK != 1);
}

void initPort()
{
    ANSELA = 0x0000;
    LATA = 0x0000;
    TRISA = 0x0010;
    ODCA = 0;
    CNENA = 0;
    CNPUA = 0;
    CNPDA = 0;

    ANSELB = 0x0000;
    LATB = 0x0008;
    TRISB = 0x01f4;
    ODCB = 0x0060;
    CNENB = 0;
    CNPUB = 0x0180;
    CNPDB = 0;

    RPINR18 = 0x0022; // RX1 = RPI34

    RPOR0 = 0x0100; // RP35 = TX1
    RPOR3 = 0x1000; // RP41 = OC1
    RPOR4 = 0x0011; // RP42 = OC2

    pswV = P_PSW;
    sswV = P_SSW;
    pswC = 0;
    sswC = 0;
    pswF = 0;
    sswF = 0;
}

void initPmd()
{
    PMD1 = 0x04da;
    PMD2 = 0x0f00;
    PMD3 = 0x0480;
    PMD4 = 0x000c;
    PMD6 = 0x0000;
    PMD7 = 0x0018;
}

void initPwm()
{
    // まだ
    PTCON2 = 0x0003;
    PTPER = 35001;

    PWMCON1 = PWMCON2 = PWMCON3 = 0x0000;
    ALTDTR1 = ALTDTR2 = ALTDTR3 = 0;
    DTR1 = DTR2 = DTR3 = 0;
    IOCON1 = IOCON2 = IOCON3 = 0x4400;

    PTCON = 0x8000;

    PDC1 = 0;
    PDC2 = 0;
    PDC3 = 0;

    T2CON = 0x8010;

    OC2TMR = 0;
    OC2RS = 17500;
    OC2R = 0;
    OC2CON2 = 0x001f;
    OC2CON1 = 0x0006;
    OC2CON2 = 0x001f;
}

void initAd()
{
    // AD0, AD1をスキャン
    AD1CON1 = 0x04ec;
    AD1CON2 = 0x0402;
    AD1CON3 = 0x9fff;
    AD1CON4 = 0x0000;
    AD1CHS123 = 0x0000;
    AD1CHS0 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0003;
}

void initUART()
{
    // 9600bps
    U1MODE = 0x8000;
    U1STA = 0x8400;
    U1BRG = 455;
}

void initT1()
{
    T1CON = 0x0010;
    PR1 = 8750;
    TMR1 = 0;
    T1CON |= 0x8000;
}

void initT3()
{
    T3CON = 0x0030;
    PR3 = 30000;
    TMR3 = 0;
    T3CON |= 0x8000;
}

void initOC1()
{
    OC1TMR = 0;
    OC1RS = 7500;
    OC1R = 0;
    OC1CON2 = 0x0000;
    OC1CON1 = 0x1c06;
}

void initInt()
{
    INTCON1 = 0x8000;
    INTCON2 = 0x8000;
    INTCON3 = 0x0000;
    INTCON4 = 0x0000;

    IEC0 = 0x1908; // U1TXIE, U1RXIE, T3IE, T1IE
    IPC0 = 0x4444; // T1
    IPC3 = 0x4443; // T1
    IFS0 = 0x0000; // T1
}

void stopMotor()
{
    PWML = PWM_STOP;
    PWMR = PWM_STOP / 2;
    PWMF = PWM_STOP;
    PWMB = PWM_STOP;
}

void beep(unsigned int wait)
{
    OC1R = 750;
    while (i < wait);
    i = 0;
    OC1R = 0;
}
