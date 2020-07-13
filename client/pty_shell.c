#include "apue.h"
#include "unp.h"
#include "common.h"
#include "cmfc.h"

void startshell(int *fd1, int *fd2)
{
    pid_t cpid;
    char *argv[] = {"bash", "bash", 0};
    if ((cpid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (cpid == 0)
    {
        close(fd1[1]);
        close(fd2[0]);
        if (fd1[0] != STDIN_FILENO)
        {
            if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
                err_sys("dup2 error to stdin");
            close(fd1[0]);
        }

        if (fd2[1] != STDOUT_FILENO)
        {
            if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
                err_sys("dup2 error to stdout");
            close(fd2[1]);
        }
        pty_main(2, argv);
        exit(0);
    }
    close(fd1[0]);
    close(fd2[1]);
    sleep(1);
    if (writen(fd1[1], "unset PROMPT_COMMAND\n", strlen("unset PROMPT_COMMAND\n")) != strlen("unset PROMPT_COMMAND\n"))
        err_sys("writen error to master pty");
}