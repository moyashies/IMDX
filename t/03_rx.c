#include <string.h>
#include "nanotap.h"
#include "../h/receive.h"

struct rx rx;

void rxSetDefault()
{
    char c;

    rxBegin();
    for (c = 'a'; c < 'f'; c++) {
        rxIndex(c);
        rxNum('1');
        rxNum('2');
        rxNum('8');
    }
    rxCommit();
}

void rxIsDeeply(struct rx *expect)
{
    int i;

    for (i = 0; i < RX_BUFSIZE; i++) {
        ok(rx.buf[i] == expect->buf[i], "rx.buf");
    }
    ok(rx.ok == expect->ok, "rx.ok");
}

int main()
{
    rxSetDefault();
    rxIsDeeply(&(struct rx){
        .buf = {
            [0] = 128,              // unused. but it is required :(
            [RX_ROTATE] = 128,
            [RX_HANDLE_X] = 128,
            [RX_HANDLE_Y] = 128,
            [RX_THROTTLE] = 128,
        },
        .ok = 1
    });

    rxBegin();
    rxIndex('a');
    rxNum('1');
    rxNum('2');
    rxNum('8');
    ok(rx.ok == 0, "rx.ok");
    rxCommit();
    rxIsDeeply(&(struct rx){.buf = {[0] = 128}, .ok = 1});

    done_testing();
    return 0;
}

