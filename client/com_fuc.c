#include "unp.h"
#include "cmfc.h"

void iconv_ucs2(wchar_t *winstr, size_t *inlen, char *outstr, size_t *outlen)
{
    char *ibuf = (char *)winstr;
    char *obuf = outstr;
    size_t *ilen = inlen;
    size_t *olen = outlen;
    iconv_t iv = iconv_open("UTF-8", "UCS-2");
    iconv (iv, &ibuf, ilen, &obuf, olen);
}

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