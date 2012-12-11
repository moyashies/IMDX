#ifndef __ST_H
#define __ST_H

#define INFOF(format, ...) stf("[INFO] " format, ##__VA_ARGS__)
#ifdef IMDX_DEBUG
#  define DEBUGF(format, ...) stf("[DEBUG] " format, ##__VA_ARGS__)
#else
#  define DEBUGF
#endif

void transmit_str(const char* buf);
void st(const char* buf);
void stf(const char* format, ...);
void st128(const char* buf, char num);
void st255(const char* buf, unsigned char num);
void st32768(const char* buf, int num);
void stff(const char* buf, unsigned char num);

#endif /* __ST_H */
