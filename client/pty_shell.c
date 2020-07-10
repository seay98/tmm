#include "apue.h"
#include "unp.h"
#include "common.h"
#include "cmfc.h"

#define TBUFFSIZE 512

void startshell(int sfd, int *shfdin)
{
    pid_t cpid;
    int nread;
    char buf[TBUFFSIZE];
    MSGINFO_S msgi;
    COMMOND_S cmds;
    char *argv[] = {"bash", "bash", "-i", 0};
    int fd1[2], fd2[2];
    if (pipe(fd1) < 0 || pipe(fd2) < 0)
        err_sys("pipe error");

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
        pty_main(3, argv);
    }
    else
    {
        close(fd1[0]);
        close(fd2[1]);

        int maxfdp1;
        fd_set rset;
        FD_ZERO(&rset);
        for (;;)
        {
            FD_SET(fd2[0], &rset);
            FD_SET(sfd, &rset);
            maxfdp1 = max(sfd, fd2[0]) + 1;
            Select(maxfdp1, &rset, NULL, NULL, NULL);

            if (FD_ISSET(fd2[0], &rset))
            {
                if ((nread = read(fd2[0], buf, TBUFFSIZE)) <= 0)
                    err_sys("read error from stdin");
                // buf[nread + 1] = 0;
                memset(&msgi, 0, sizeof(MSGINFO_S));
                memset(&cmds, 0, sizeof(COMMOND_S));
                msgi.msg_id = CMD_TELNET;
                cmds.flag = 0;

                memcpy((char *)&cmds.command, (char *)&buf, nread);
                // wchar_t wcs[TBUFFSIZE];
                // swprintf(wcs, TBUFFSIZE, L"%ls", cmds.command);
                // size_t ilen = TBUFFSIZE;
                // size_t olen = TBUFFSIZE;
                // iconv_ucs2(wcs, &ilen, cmds.command, &olen);

                memcpy((char *)&msgi.context, (char *)&cmds, sizeof(COMMOND_S));
                encrypt_buf((char *)&msgi, sizeof(MSGINFO_S));
                Writen(sfd, &msgi, sizeof(MSGINFO_S));
                // writen(fd1[1], "ls\n", strlen("ls\n"));
                // sleep(1);
            }

            if (FD_ISSET(sfd, &rset))
            {
                if (Readn(sfd, &msgi, sizeof(MSGINFO_S))==0) {
                    err_quit("cli_proc: server terminated prematurely!");
                }
                decrypt_buf((char *)&msgi, sizeof(MSGINFO_S));
                COMMOND_S *cmd = (COMMOND_S *)&msgi.context;
                // printf("%s\n", cmd->command);
                if (writen(fd1[1], cmd->command, strlen(cmd->command)) != strlen(cmd->command))
                    err_sys("writen error to master pty");
                fflush(stdout);
            }
        }
    }
}