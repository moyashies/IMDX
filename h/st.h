#ifndef __ST_H
#define __ST_H

#include <xc.h>

void transmit_str(const char* buf);
void st(const char* buf);
void st128(const char* buf, char num);
void st255(const char* buf, unsigned char num);
void st32768(const char* buf, int num);
void stff(const char* buf, unsigned char num);

#endif /* __ST_H */
