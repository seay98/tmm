#include <fcntl.h>
#include "apue.h"
#include "cmfc.h"

#define RBUF    4096

int main(void)
{
    int fd;
    int n;
    char buf[RBUF];

    memset(buf, 0, RBUF);
    if ((fd = open("../lt.txt", O_RDONLY)) < 0)
        err_sys("open error");
    
    if ((n = read(fd, buf, RBUF)) < 0)
        err_sys("read error");

    close(fd);

    printf("%s\n", buf);

    encrypt_buf(buf, n);

    if ((fd = open("../lt", O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH)) < 0)
        err_sys("open error");

    if (write(fd, buf, n) != n)
        err_sys("write error");

    close(fd);
}