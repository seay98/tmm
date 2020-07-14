#include "unp.h"
#include "cmfc.h"
#include "common.h"

int fetch_sysinfo(void * msgi)
{
    MSGINFO_S * msgptr = (MSGINFO_S *)msgi;
    SYSTEMINFO_S sysinfo;
    bzero(&sysinfo, sizeof(sysinfo));

    sysinfo.OSVer = 14;

    char hostname[MAX_PATH];
    gethostname(hostname, MAX_PATH);
    memcpy(sysinfo.hostName, hostname, strlen(hostname));

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
            Inet_ntop(hptr->h_addrtype, *pptr, sysinfo.szLocalIP, sizeof(sysinfo.szLocalIP));
            break;
        default:
            break;
        }
    }

    wchar_t wcs[MAX_PATH/4];
    struct utsname hname;
    uname(&hname);
    swprintf(wcs, MAX_PATH/4, L"%s %s", hname.sysname, hname.release);
    char *ibuf = (char *)wcs;
    char *obuf = sysinfo.szCPUInfo;
    size_t ilen = MAX_PATH/4;
    size_t olen = MAX_PATH;
    iconv_t iv = iconv_open("UTF-8", "UCS-2");
    iconv (iv, &ibuf, &ilen, &obuf, &olen);

    sysinfo.dwDiskSize = 1024;

    memcpy((char *)msgptr->context, (char *)&sysinfo, CONTEXT_BUF_SIZE);
    return(1);
}