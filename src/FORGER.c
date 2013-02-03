#include <xc.h>
#include "../h/i2cEmem.h"

#include "../h/motor.h"
#include "../h/pd.h"
#include "../h/receive.h"
#include "../h/sensor.h"
#include "../h/transmit.h"

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
void initTimer();
void initOC1();
void initInt();
void stopMotor();
void beep(unsigned int wait);

// Instantiate Drive and Data objects
I2CEMEM_DRV i2cmem = I2CSEMEM_DRV_DEFAULTS;
I2CEMEM_DATA wData;
I2CEMEM_DATA rData;

unsigned int wBuff[10], rBuff[10];

unsigned int i = 0;

unsigned int pswC = 0;
unsigned int pswF = 0;
unsigned int pswV = 0;

unsigned int sswC = 0;
unsigned int sswF = 0;
unsigned int sswV = 0;

unsigned int leds = 0;
unsigned int ledCount = 0;

unsigned int receivedNum = 0;

int acce[3], gyro[3];
int angleBefore[2] = {};
int gyroBefore[3] = {};

int angle[2] = {};
int anglePd[2], gyroPd[3];

struct pwm motor;
struct rx rx;

int main(void)
{
    initOsc();
    initPort();
    initPwm();
    initAd();
    initUART();
    initTimer();
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
    beep(250);
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
        if (rx.timeout) {
            leds = 0x0f;
            continue;
        }

        if (pswF && pswV) {
            pswF = 0;
            INFOF("s");
        }
        leds ^= 0x04;
        if (rx.ok) {
            rx.ok = 0;
            leds ^= 0x08;
        }

        rData.devSel = ACCE;
        rData.addr = 0xa9;
        rData.n = 5;
        i2cmem.oData = &rData;
        i2cmem.cmd = I2C_READ;
        while (i2cmem.cmd != I2C_IDLE) {
            i2cmem.tick(&i2cmem);
        }

        // low-pass filter
        acce[0] = 0.9 * acce[0] + 0.1 * (char)rBuff[0];
        acce[1] = 0.9 * acce[1] + 0.1 * (char)rBuff[2];
        acce[2] = 0.9 * acce[2] + 0.1 * (char)rBuff[4];

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

        setAngleX();
        setAngleY();

        stf("A,%d,%d,%d,"
            "%d,%d,%d,"
            "%d,%d\n",
            acce[0], acce[1], acce[2],
            gyro[0], gyro[1], gyro[2],
            angle[0], angle[1]);

        stf("M,%d,%d,%d,%d\n",
            motor.left, motor.right, motor.front, motor.back);
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
        rxBegin();
        break;
    case 'r':
        rxCommit();
        break;
    case 'a':
    case 'b': /* rotate */
    case 'c': /* handle x */
    case 'd': /* handle y */
    case 'e': /* throttle */
    case 'f': /* trigger */
    case 'g':
    case 'h': /* angle KP */
    case 'i': /* angle KD */
    case 'j': /* gyro KP */
    case 'k': /* gyro KD */
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
        rxIndex(c);
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
        rxNum(c);
        break;
    }
}

void _ISR _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;

    if (!txIsEmpty()) {
        U1TXREG = txPop();
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

    // motorEnable
    if (sswV && !rx.timeout) {
        if (rx.ok) {
            rx.ok = 0;
            if (rx.buf[RX_TRIGGER] == 1) {
                setAnglePd();
                angleBefore[0] = angle[0];
                angleBefore[1] = angle[1];

                setGyroPd();
                gyroBefore[0] = gyro[0];
                gyroBefore[1] = gyro[1];
                gyroBefore[2] = gyro[2];

                motorStart();
            } else {
                motorStop();
            }
        }
    } else {
        motorStop();
    }

    PWML = motor.left;
    PWMR = motor.right / 2;
    PWMF = motor.front;
    PWMB = motor.back;
}

void _ISRFAST _T3Interrupt(void)
{
    IFS0bits.T3IF = 0;
}

void _ISRFAST _T4Interrupt(void)
{
    IFS1bits.T4IF = 0;

    rx.timeout = receivedNum == 0 ? 1 : 0;
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

    pswV = P_PSW ? 1 : 0;
    sswV = P_SSW ? 1 : 0;
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

void initTimer()
{
    // PWM
    T1CON = 0x0010; // 1:8
    PR1 = 8750;     // 1000 Hz
    TMR1 = 0;
    T1CON |= 0x8000;

    // acce, gyro sensor
    T3CON = 0x0020; // 1:64
    PR3 = 10937;    // 100 Hz
    TMR3 = 0;
    T3CON |= 0x8000;

    // timeout
    T4CON = 0x0030; // 1:256
    PR4 = 54687;    // 5 Hz
    TMR4 = 0;
    T4CON |= 0x8000;
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

    IEC0 = 0x1808; // U1TXIE, U1RXIE, disabled(T3IE), T1IE
    IEC1 = 0x0800; // T4IE
    IPC0 = 0x4444;
    IPC3 = 0x4443; // U1TXIP
    IPC6 = 0x4444; // T4IP
    IFS0 = 0x0000; // T1
    IFS1 = 0x0000;
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
