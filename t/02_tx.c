#include "nanotap.h"
#include "../h/transmit.h"

void txVomit(char *buf, int size)
{
    int i;

    for (i = 0; i < size; i++) {
        buf[i] = txPop();
    }
    buf[i] = '\0';
}

int main()
{
    txPush('a');
    ok(!txIsEmpty(), "not empty");
    ok(txPop() == 'a', "a");
    ok(txIsEmpty(), "empty");

    {
        char buf[128];

        txPush('I');
        txPush('M');
        txPush('D');
        ok(!txIsEmpty(), "not empty");
        txVomit(buf, 3);
        contains_string(buf, "IMD", "IMD");
        ok(txIsEmpty(), "empty");
    }

    {
        char buf[128];

        transmitStr("hello, world!");
        ok(!txIsEmpty(), "not empty");
        txVomit(buf, 13);
        contains_string(buf, "hello, world!", "hello, world!");
        ok(txIsEmpty(), "empty");
    }

    {
        char buf[128];

        stf("http://moyashies.com/%d/%s", 2013, "IMDX");
        ok(!txIsEmpty(), "not empty");
        txVomit(buf, 30);
        contains_string(buf, "http://moyashies.com/2013/IMDX", "vomit");
        ok(txIsEmpty(), "empty");
    }

    {
        char buf[128];

        INFOF("ignition!");
        ok(!txIsEmpty(), "not empty");
        txVomit(buf, 38);
        contains_string(buf, "[INFO] ignition! at t/02_tx.c line 56\n", "INFO");
        ok(txIsEmpty(), "empty");
    }

    {
        /* 128 characters */
        char str[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        char buf[128];

        transmitStr(str);
        ok(!txIsEmpty(), "not empty");
        txVomit(buf, 128);
        contains_string(buf, str, "128 characters");
        ok(txIsEmpty(), "empty");
    }

    done_testing();
    return 0;
}
