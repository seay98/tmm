#ifndef	__cmfc_h
#define	__cmfc_h

#include <unistd.h>
#include <sys/utsname.h>
#include <wchar.h>
#include <iconv.h>

void cli_proc(const int *);
int fetch_sysinfo(void *);
int fetch_sysinfos(char *);
int pty_main(int, char *[]);
int startshell(int*, int*);
int get_servaddr(char *, short *);

int http_down(const char *, char *);
int http_read(const char *, char *);
int http_post(const char *, char *, char *);
int http_postb(const char *, char *, size_t, char *);

void encrypt_buf(char *, size_t);
void decrypt_buf(char *, size_t);

#endif