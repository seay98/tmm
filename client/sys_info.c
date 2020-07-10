#include "unp.h"
#include "cmfc.h"
#include "common.h"

bool fetch_sysinfo(void * msgi)
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

    wchar_t wcs[512];
    struct utsname hname;
    uname(&hname);
    swprintf(wcs, 512, L"%s %s", hname.sysname, hname.release);
    size_t ilen = 512;
    size_t olen = 512;
    iconv_ucs2(wcs, &ilen, sysinfo.szCPUInfo, &olen);

    sysinfo.dwDiskSize = 1024;

    memcpy((char *)msgptr->context, (char *)&sysinfo, CONTEXT_BUF_SIZE);
    return true;
}