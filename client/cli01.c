#include "unp.h"
#include "cmfc.h"

int main (int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    char servip[24];
    short port = 4433;

    get_servaddr(servip, &port);
    
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    Inet_pton(AF_INET, servip, &servaddr.sin_addr.s_addr);
    
    Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

    cli_proc(sockfd);

    exit(0);
}