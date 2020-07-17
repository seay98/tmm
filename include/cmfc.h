#ifndef	__cmfc_h
#define	__cmfc_h

#include <unistd.h>
#include <sys/utsname.h>
#include <wchar.h>
#include <iconv.h>

void cli_proc(const int *);
int fetch_sysinfo(void *);
int pty_main(int, char *[]);
int startshell(int*, int*);
int get_servaddr(char *, short *);

void encrypt_buf(char *, size_t);
void decrypt_buf(char *, size_t);

#endif