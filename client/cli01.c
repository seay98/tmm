#include "unp.h"
#include "cmfc.h"

int Conn(int fd, const struct sockaddr *sa, socklen_t salen)
{
    int res;
	res = connect(fd, sa, salen);
        // printf("conn err\n");
    return res;
}

int main(int argc, char **argv)
{
    int sn = 0;
    int sockfd;
    struct sockaddr_in servaddr;

    char servip[24];
    short port = 4433;

    for (;;)
    {
        if ((get_servaddr(servip, &port)) == 0)
        {
            sockfd = Socket(AF_INET, SOCK_STREAM, 0);

            bzero(&servaddr, sizeof(servaddr));
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(port);
            Inet_pton(AF_INET, servip, &servaddr.sin_addr.s_addr);

            if (Conn(sockfd, (SA *)&servaddr, sizeof(servaddr)) == 0)
            {
                cli_proc(&sockfd);
                close(sockfd);
            }
        }
        sleep(sn + 30);
        // sn += 10;
        // if (sn > 600)
        //     sn = 0;
    }
}