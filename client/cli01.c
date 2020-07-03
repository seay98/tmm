#include "unp.h"
#include "cmfc.h"

int main (int argc, char **argv)
{
    printf("cli set off\n");
    int sockfd;
    struct sockaddr_in servaddr;
    
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, "10.1.199.161", &servaddr.sin_addr.s_addr);
    
    Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

    cli_proc(sockfd);

    exit(0);
}