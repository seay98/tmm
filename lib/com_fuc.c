#include "unp.h"
#include "cmfc.h"

void encrypt_buf(char *buf, size_t len)
{
    for (int i=0; i<len; i++) {
        u_char ch = 0;
        ch = (buf[i] >> 4) | (buf[i] << 4);
        ch = (ch << 3) | (ch >> 5);
        buf[i] = ch ^ 0x23;
    }
}

void decrypt_buf(char *buf, size_t len)
{
    for (int i=0; i<len; i++) {
        u_char ch = buf[i] ^ 0x23;
        ch = (ch >> 3) | (ch << 5);
        buf[i] = ( ch >> 4) | (ch << 4);
    }
}

void y23_buf(char *buf, size_t len)
{
    for (int i=0; i<len; i++) {
        buf[i] ^= 0x23;
    }
}