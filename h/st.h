#ifndef __ST_H
#define __ST_H

#define _LOG(format, ...) \
    stf(format "at %s line %d\n", ##__VA_ARGS__, __FILE__, __LINE__)
#define INFOF(format, ...) _LOG("[INFO] " format, ##__VA_ARGS__)
#define DEBUGF
#ifdef IMDX_DEBUG
#  define DEBUGF(format, ...) _LOG("[DEBUG] " format, ##__VA_ARGS__)
#endif

void transmitStr(const char* buf);
void st(const char* buf);
void stf(const char* format, ...);

#endif /* __ST_H */
