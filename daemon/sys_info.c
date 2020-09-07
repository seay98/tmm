#include "unp.h"
#include "cmfc.h"
#include "common.h"

int fetch_sysinfos(char *syin)
{
    char hostname[MAX_PATH];
    gethostname(hostname, MAX_PATH);

    char ipaddr[32];
    char **pptr;
    struct hostent *hptr;
    if ( (hptr = gethostbyname(hostname)) == NULL ) {
        err_msg("gethostbyname error for host: %s: %s", hostname, hstrerror(h_errno));
    }
    else {
        switch (hptr->h_addrtype) 
        {
        case AF_INET:
            pptr = hptr->h_addr_list;
            Inet_ntop(hptr->h_addrtype, *pptr, ipaddr, sizeof(ipaddr));
            break;
        default:
            break;
        }
    }

    struct utsname hname;
    uname(&hname);

    // sprintf(syin, "hostname=1&os=2 2&ip=3");
    // sprintf(syin, "{\"hostname\":\"%s\",\"os\":\"%s %s\",\"ip\":\"%s\"}", hostname, hname.sysname, hname.release, ipaddr);
    sprintf(syin, "{\"hostname\":\"%s\",\"os\":\"%s %s\"}", hostname, hname.sysname, hname.release);
    return(0);
}