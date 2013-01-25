#ifndef __TRANSMIT_H
#define __TRANSMIT_H

#define _LOG(format, ...) \
    stf(format " at %s line %d\n", ##__VA_ARGS__, __FILE__, __LINE__)
#define INFOF(format, ...) _LOG("[INFO] " format, ##__VA_ARGS__)
#define DEBUGF
#ifdef IMDX_DEBUG
#  define DEBUGF(format, ...) _LOG("[DEBUG] " format, ##__VA_ARGS__)
#endif

char txPop();
void txPush(char c);
unsigned char txIsEmpty();

void transmitStr(const char *buf);
void stf(const char* format, ...);

#endif /* __TRANSMIT_H */
