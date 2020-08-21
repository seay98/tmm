#include "apue.h"
#include <syslog.h>
#include <errno.h>
#include <sys/wait.h>
#include <pthread.h>
#include "cmfc.h"

#define PROGFILE "/tmp/cli01"
#define DOMAIN  "http://www.gordeneyes.com/"
#define TESTDOMAIN  "http://127.0.0.1:3000/"
#define VERSIONINFO "http://www.gordeneyes.com/infos/v"
#define REMOTEFILE  "http://www.gordeneyes.com/temp/cli01"
#define PFLAG   "@#$:"
#define CMDSIZE 64
#define BSIZE 1024
#define RWXXX   (S_IRWXU|S_IXGRP|S_IXOTH)
#define RWX   (S_IRWXU)

extern int lockfile(int);
extern int already_running(void);

struct thrupds
{
    char *md5val;
    pid_t *pid;
};

void sigterm(int signo)
{
    syslog(LOG_INFO, "got SIGTERM; exiting");
    exit(0);
}

void *thr_upd(void *arg)
{
    struct thrupds *param = (struct thrupds *)arg;
    char buf[BSIZE];
    int n = 0;
    for (;;)
    {
        memset(buf, 0, BSIZE);
        if (http_read(VERSIONINFO, buf) == 0)
        {
            if ((n = strlen(buf)) > 32)
            {
                if ((n = strncmp(param->md5val, buf, 32)) != 0)
                {
                    kill(*param->pid, SIGTERM);
                    syslog(LOG_ERR, "need update");
                    break;
                }
                // syslog(LOG_ERR, "%d::%s::%s", n, param->md5val, buf);
            }
        }
        sleep(60);
    }
    return ((void *)0);
}

int get_prog(char *prog, char *argv[], const char *buf)
{
    char *finder;
    char *iter;
    int n;

    strcpy(prog, buf);
    if ((finder = strstr(prog, ":")) == NULL)
    {
        return (-1);
    }

    // prog
    finder[0] = 0;
    if (finder[1] == 0)
    {
        return(0);
    }

    // argv
    iter = finder + 1;
    n = 0;
    argv[n++] = "clid";
    while ((finder = strstr(iter, " ")) != NULL)
    {
        argv[n++] = iter;
        iter = finder + 1;
        finder[0] = 0;
    }
    argv[n] = NULL;
    return(0);
}

int main(int argc, char *argv[])
{
    char *cmd;
    struct sigaction sa;

    char *progv[BSIZE];
    char prog[BSIZE];
    char *tmptr;

    pid_t pid;
    pthread_t tid;
    int res, pexist, chkok, rmtok, loclok, N;
    int status; // -1: preparing, 0: ready, 1: need download
    size_t nread1, nread2;
    FILE *fpin;
    struct thrupds tus;
    char cmdstr[CMDSIZE];
    char cmdres[BSIZE];
    char rbuf[BSIZE];
    char wbuf[BSIZE];
    char postbody[BSIZE];

    // if ((cmd = strrchr(argv[0], '/')) == NULL)
    //     cmd = argv[0];
    // else
    //     cmd++;

    // /*
	//  * Become a daemon.
	//  */
    // daemonize(cmd);

    // /*
	//  * Make sure only one copy of the daemon is running.
	//  */
    // if (already_running())
    // {
    //     syslog(LOG_ERR, "daemon already running");
    //     exit(1);
    // }
    // /*
	//  * Handle signals of interest.
	//  */
    // sa.sa_handler = sigterm;
    // sigemptyset(&sa.sa_mask);
    // sigaddset(&sa.sa_mask, SIGTERM);
    // sa.sa_flags = 0;
    // if (sigaction(SIGTERM, &sa, NULL) < 0)
    // {
    //     syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));
    //     exit(1);
    // }

    /*
	 * Proceed with the rest of the daemon.
	 */
    for (;;)
    {
        memset(rbuf, 0, BSIZE);
        memset(wbuf, 0, BSIZE);
        memset(postbody, 0, BSIZE);
        strcpy(wbuf, TESTDOMAIN);
        strcat(wbuf, "moien");
        printf("%s\n", wbuf);
        fetch_sysinfos(postbody);
        // strcpy(postbody, "os=linux&ip=1.1.1.1");
        http_postb(wbuf, postbody, strlen(postbody), rbuf);
        // http_post(wbuf, "os=linux", rbuf);
        printf("%s\n", rbuf);
        break;
        sleep(60);
    }
    // N = 0;
    // status = -1;
    // for (;;)
    // {
    //     pid = -1;
    //     res = -1;
    //     pexist = -1;
    //     nread1 = -1;
    //     nread2 = -1;

    //     // fetch configure from domain
    //     strcpy(prog, PROGFILE);
    //     progv[0] = "clid";
    //     progv[1] = ">/dev/null";
    //     progv[2] = "2>&1";
    //     progv[3] = "&";
    //     progv[4] = NULL;
    //     rmtok = -1;
    //     memset(rbuf, 0, BSIZE);
    //     if ((res = http_read(VERSIONINFO, rbuf)) < 0)
    //     {
    //         syslog(LOG_ERR, "curl read error: %s", strerror(errno));
    //         status = 0;
    //     }
    //     if (res == 0)
    //     {
    //         // remote 404
    //         if ((strstr(rbuf, "<html>") != NULL) || ((nread2 = strlen(rbuf)) < 32))
    //         {
    //             syslog(LOG_ERR, "conf fetch error: %ld %s", nread2, rbuf);
    //             status = 0;
    //         }
    //         else
    //         {
    //             // read parameters
    //             if ((tmptr = strstr(rbuf, PFLAG)) != NULL)
    //             {
    //                 if (get_prog(prog, progv, tmptr + strlen(PFLAG)) < 0)
    //                 {
    //                     syslog(LOG_ERR, "conf parse error: %ld %s", nread2, rbuf);
    //                 }
    //             }
    //             rmtok = 0;
    //         }
    //     }
    //     syslog(LOG_ERR, "%s", prog);

    //     // if prog exists, calulate its md5 value
    //     loclok = -1;
    //     if ((pexist = access(prog, F_OK)) == 0)
    //     {
    //         strcpy(cmdstr, "md5sum ");
    //         strcat(cmdstr, prog);
    //         if ((fpin = popen(cmdstr, "r")) != NULL)
    //         {
    //             memset(cmdres, 0, BSIZE);
    //             if ((nread1 = fread(cmdres, 1, BSIZE, fpin)) < 32)
    //             {
    //                 syslog(LOG_ERR, "popen read error: %s", strerror(errno));
    //             }
    //             else
    //             {
    //                 loclok = 0;
    //             }
    //             if (pclose(fpin) == -1)
    //                 syslog(LOG_ERR, "pclose error: %s", strerror(errno));
    //         }
    //     }

    //     // compare md5 values
    //     chkok = -1;
    //     if (rmtok == 0 && loclok == 0)
    //     {
    //         if (strncmp(cmdres, rbuf, 32) == 0)
    //         {
    //             chkok = 0;
    //             status = 0;
    //         }
    //         syslog(LOG_ERR, "%s::%s", cmdres, rbuf);
    //     }

    //     if (pexist != 0 || (rmtok == 0 && chkok != 0))
    //     {
    //         status = 1;
    //     }

    //     if (status == 1)
    //     {
    //         if ((res = remove(prog)) < 0)
    //         {
    //             // do nothing;
    //         }
    //         if ((res = http_down(REMOTEFILE, prog)) < 0)
    //         {
    //             syslog(LOG_ERR, "download error: %s", strerror(errno));
    //         }
    //         status = -1;
    //     }

    //     if (status == 0)
    //     {
    //         if ((pid = fork()) < 0)
    //         {
    //             err_sys("fork error");
    //         }
    //         else if (pid == 0)
    //         {
    //             if (access(prog, X_OK) < 0) {
    //                 chmod(prog, RWX);
    //             }
    //             execv(prog, progv);
    //         }
    //         else
    //         {
    //             tus.md5val = cmdres;
    //             tus.pid = &pid;
    //             res = pthread_create(&tid, NULL, thr_upd, (void *)&tus);
    //             if (res != 0)
    //             {
    //                 syslog(LOG_ERR, "update thread create error: %s", strerror(errno));
    //             }

    //             waitpid(pid, NULL, 0);
    //         }
    //     }
    //     sleep(N * 60);
    //     N++;
    //     if (N > 10)
    //         N = 0;
    // }

    exit(0);
}