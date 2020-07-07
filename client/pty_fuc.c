#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#if defined(SOLARIS)
#include <stropts.h>
#endif

int ptym_open(char *pts_name, int pts_namesz)
{
    char *ptr;
    int fdm, err;

    if ((fdm = posix_openpt(O_RDWR)) < 0)
        return(-1);
    // grant access to slave
    if (grantpt(fdm) < 0)
        goto errout;
    // clear slave's lock flag
    if (unlockpt(fdm) < 0)
        goto errout;
    // get slave's name
    if ((ptr = ptsname(fdm)) == NULL)
        goto errout;

    strncpy(pts_name, ptr, pts_namesz);
    pts_name[pts_namesz - 1] = '\0';
    return(fdm);
errout:
    err = errno;
    close(fdm);
    errno = err;
    return(-1);
}

int ptys_open(char *pts_name)
{
    int fds;
#if defined(SOLARIS)
    int err, setup;
#endif

    if ((fds = open(pts_name, O_RDWR)) < 0)
        return(-1);

#if defined(SOLARIS)
    if ((setup = ioctl(fds, I_FIND, "ldterm")) < 0)
        goto errout;
    
    if (setup == 0) {
        if (ioctl(fds, I_PUSH, "ptem") < 0)
            goto errout;
        if (ioctl(fds, I_PUSH, "ldterm") < 0)
            goto errout;
        if (ioctl(fds, I_PUSH, "ttcompat") < 0) {
errout:
            err = errno;
            close(fds);
            errno = err;
            return(-1);
        }
    }
#endif

    return(fds);
}

pid_t pty_fork(int *ptrfdm, char *slave_name, int slave_namesz,
                        const struct termios *slave_termios,
                        const struct winsize *slave_winsize)
{
    int fdm, fds;
    pid_t pid;
    char pts_name[20];

    if ((fdm = ptym_open(pts_name, sizeof(pts_name))) < 0)
        err_sys("can't open master pty: %s, error %d", pts_name, fdm);
    
    if (slave_name != NULL) {
        strncpy(slave_name, pts_name, slave_namesz);
        slave_name[slave_namesz - 1] = '\0';
    }

    if ((pid = fork()) < 0) {
        return(-1);
    }else if (pid == 0) {
        // create new session
        if (setsid() < 0)
            err_sys("setsid error");

        if ((fds = ptys_open(pts_name)) < 0)
            err_sys("can't open slave pty");
        
        close(fdm);

#if defined(BSD)
        if (ioctl(fds, TIOCSCTTY, (char *)0) < 0)
            err_sys("TIOCSCTTY error");
#endif
        
        if (slave_termios != NULL) {
            if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
                err_sys("tcsetattr error on slave pty");
        }
        if (slave_winsize != NULL) {
            if (ioctl(fds, TIOCSWINSZ, slave_termios) < 0)
                err_sys("TIOCSWINSZ error on slave pty");
        }

        if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
            err_sys("dup2 error in stdin");
        if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
            err_sys("dup2 error in stdout");
        if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
            err_sys("dup2 error in stderr");
        if (fds != STDIN_FILENO && fds != STDOUT_FILENO && fds != STDERR_FILENO)
            close(fds);
        return(0);
    } else {
        *ptrfdm = fdm;
        return(pid);
    }
}