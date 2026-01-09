// mailbox.h
#define VIDEOCORE_MBOX 0x3F00B880
#define MBOX_READ      (VIDEOCORE_MBOX + 0x00)
#define MBOX_STATUS    (VIDEOCORE_MBOX + 0x18)
#define MBOX_WRITE     (VIDEOCORE_MBOX + 0x20)
#define MBOX_RESPONSE  0x80000000
#define MBOX_CH_PROP   8
#define MBOX_TAG_GET_MOUSE 0x00040005
extern volatile unsigned int mbox[36];
int mbox_call(unsigned char ch);