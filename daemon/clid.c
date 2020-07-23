#include "apue.h"
#include <syslog.h>
#include <errno.h>
#include "cmfc.h"

#define BFILE "/home/shw/dev/tmm/client/cli01"
#define CMDSIZE 64
#define BSIZE   256

extern int lockfile(int);
extern int already_running(void);

void sigterm(int signo)
{
    syslog(LOG_INFO, "got SIGTERM; exiting");
    exit(0);
}

int main(int argc, char *argv[])
{
    char *cmd;
    struct sigaction sa;

    pid_t pid;
    int res;
    size_t nread;
    FILE *fpin;
    char cmdstr[CMDSIZE];
    char cmdres[BSIZE];
    char rbuf[BSIZE];

    if ((cmd = strrchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else
        cmd++;

    /*
	 * Become a daemon.
	 */
    daemonize(cmd);

    /*
	 * Make sure only one copy of the daemon is running.
	 */
    if (already_running())
    {
        syslog(LOG_ERR, "daemon already running");
        exit(1);
    }
    /*
	 * Handle signals of interest.
	 */
    sa.sa_handler = sigterm;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGTERM);
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, NULL) < 0)
    {
        syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));
        exit(1);
    }

    /*
	 * Proceed with the rest of the daemon.
	 */
    if (access(BFILE, F_OK) == 0)
    {
        strcpy(cmdstr, "md5sum ");
        strcat(cmdstr, BFILE);
        if ((fpin = popen(cmdstr, "r")) != NULL)
        {
            memset(cmdres, 0, BSIZE);
            if ((nread = fread(cmdres, 1, BSIZE, fpin)) != nread){
                syslog(LOG_ERR, "popen read error: %s", strerror(errno));
            }
            syslog(LOG_ERR, "md5: %s", cmdres);
        }
    }
    // if ((res = http_down("http://www.gordeneyes.com/tmp/4e.pdf", "/home/shw/4e.pdf")) < 0)
    // {
    //     syslog(LOG_ERR, "download error: %s", strerror(errno));
    // }

    // if ((pid = fork()) < 0) {
    //     err_sys("fork error");
    // } else if (pid == 0) {
    //     execl("/home/shw/dev/tmm/client/cli01", ">/dev/null", "2>&1", "&", NULL);
    // }
    // for (;;) {
    //     sleep(3);
    // }

    exit(0);
}