#include "unp.h"
#include "common.h"
#include "cmfc.h"

void cli_proc(int sockfd)
{
    int maxfdp1;
    fd_set rset;
    MSGINFO_S msgi;

    FD_ZERO(&rset);
    bzero(&msgi, sizeof(MSGINFO_S));
    for ( ; ; ) {
        FD_SET(sockfd, &rset);
        maxfdp1 = sockfd + 1;
        Select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            if (Readn(sockfd, &msgi, sizeof(MSGINFO_S))==0) {
                err_quit("cli_proc: server terminated prematurely!");
            }
            
            char *beacon = "Jo*Po*Hello";
            if ((CMD_SHAKEHAND == msgi.msg_id) && (0 == strcmp(beacon, (char *)msgi.context))) {
                bzero(&msgi, sizeof(MSGINFO_S));
                bool res = fetch_sysinfo(&msgi);
                msgi.msg_id = CMD_SYSINFO;
                if (res) {
                    encrypt_buf((char *)&msgi, sizeof(MSGINFO_S));
                    Writen(sockfd, &msgi, sizeof(MSGINFO_S));
                }
            }
            else {
                decrypt_buf((char *)&msgi, sizeof(MSGINFO_S));
                printf("0x%02x\n", msgi.msg_id);
                COMMOND_S *cmd = (COMMOND_S *)&msgi.context;
                printf("%s\n", cmd->command);

                char *argv[] = {"./cli01", "bash", 0};
                pid_t cpid;
                if ((cpid = fork()) < 0) {
                    err_sys("fork error");
                }else if (cpid == 0) {
                    pty_main(3, argv, sockfd);
                    // char buf[] = "bash:#";
                    // bzero(&msgi, sizeof(MSGINFO_S));
                    // msgi.msg_id = CMD_TELNET;
                    // COMMOND_S cmds;
                    // bzero(&cmds, sizeof(COMMOND_S));
                    // cmds.flag = 0;
                    // memcpy((char *)&cmds.command, (char *)&buf, strlen(buf));
                    // memcpy((char *)&msgi.context, (char *)&cmds, sizeof(COMMOND_S));
                    // encrypt_buf((char *)&msgi, sizeof(MSGINFO_S));
                    // Writen(sockfd, &msgi, sizeof(MSGINFO_S));
                    // printf("pty fork done\n");
                    exit(0);
                }
                printf("pty read\n");
            }
        }
    }
}