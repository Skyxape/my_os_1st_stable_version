// mailbox.c
#include "mailbox.h"
volatile unsigned int __attribute__((aligned(16))) mbox[36];
int mbox_call(unsigned char ch) {
    unsigned int r = (((unsigned int)((unsigned long)&mbox) & ~0xF) | (ch & 0xF));
    while (*(volatile unsigned int *)MBOX_STATUS & 0x80000000);
    *(volatile unsigned int *)MBOX_WRITE = r;
    while (1) {
        while (*(volatile unsigned int *)MBOX_STATUS & 0x40000000);
        if (r == *(volatile unsigned int *)MBOX_READ) return mbox[1] == MBOX_RESPONSE;
    }
}