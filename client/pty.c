#include "apue.h"
#include "unp.h"
#include "common.h"
#include <termios.h>

#ifdef LINUX
#define OPTSTR "+d:einv"
#else
#define OPTSTR "d:einv"
#endif

#define	TBUFFSIZE	512

static void	sig_term(int);
static volatile sig_atomic_t	sigcaught;	/* set by signal handler */

static void	set_noecho(int);	/* at the end of this file */
void do_driver(char *);	/* in the file driver.c */
void loop(int, int);	

int pty_main(int argc, char *argv[])
{
    int fdm, c, ignoreeof, interactive, noecho, verbose;
	pid_t pid;
	char *driver;
	char slave_name[20];
	struct termios orig_termios;
	struct winsize size;

	interactive = isatty(STDIN_FILENO);
	ignoreeof = 0;
	noecho = 0;
	verbose = 0;
	driver = NULL;

    opterr = 0;		/* don't want getopt() writing to stderr */
	while ((c = getopt(argc, argv, OPTSTR)) != EOF) {
		switch (c) {
		case 'd':		/* driver for stdin/stdout */
			driver = optarg;
			break;

		case 'e':		/* noecho for slave pty's line discipline */
			noecho = 1;
			break;

		case 'i':		/* ignore EOF on standard input */
			ignoreeof = 1;
			break;

		case 'n':		/* not interactive */
			interactive = 0;
			break;

		case 'v':		/* verbose */
			verbose = 1;
			break;

		case '?':
			err_quit("unrecognized option: -%c", optopt);
		}
	}

    if (optind >= argc)
		err_quit("usage: pty [ -d driver -einv ] program [ arg ... ]");

    if (interactive) {	/* fetch current termios and window size */
		if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)
			err_sys("tcgetattr error on stdin");
		if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *) &size) < 0)
			err_sys("TIOCGWINSZ error");
		pid = pty_fork(&fdm, slave_name, sizeof(slave_name), &orig_termios, &size);
	} else {
		// memset(&orig_termios, 0, sizeof(struct termios));
		// orig_termios.c_iflag |= IUTF8;
		pid = pty_fork(&fdm, slave_name, sizeof(slave_name), &orig_termios, NULL);
	}

    if (pid < 0) {
		err_sys("fork error");
	} else if (pid == 0) {		/* child */
		if (noecho)
			set_noecho(STDIN_FILENO);	/* stdin is slave pty */

		if (execvp(argv[optind], &argv[optind]) < 0)
		// if (execl("/bin/bash", "bash", (char*)0) < 0)
			err_sys("can't execute: %s", argv[optind]);
	}

	// printf("child: %d parent: %d\n", pid, getppid());
    if (verbose) {
		fprintf(stderr, "slave name = %s\n", slave_name);
		if (driver != NULL)
			fprintf(stderr, "driver = %s\n", driver);
	}

	if (interactive && driver == NULL) {
		if (tty_raw(STDIN_FILENO) < 0)	/* user's tty to raw mode */
			err_sys("tty_raw error");
		if (atexit(tty_atexit) < 0)		/* reset user's tty on exit */
			err_sys("atexit error");
	}

	// if (driver)
	// 	do_driver(driver);	/* changes our stdin/stdout */

	loop(fdm, ignoreeof);	/* copies stdin -> ptym, ptym -> stdout */

	return(fdm);
}

static void set_noecho(int fd)		/* turn off echo (for slave pty) */
{
	struct termios	stermios;

	if (tcgetattr(fd, &stermios) < 0)
		err_sys("tcgetattr error");

	stermios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);

	/*
	 * Also turn off NL to CR/NL mapping on output.
	 */
	stermios.c_oflag &= ~(ONLCR);

	if (tcsetattr(fd, TCSANOW, &stermios) < 0)
		err_sys("tcsetattr error");
}

void loop(int ptym, int ignoreeof)
{
	pid_t	child;
	int		nread;
	char	buf[TBUFFSIZE];

	if ((child = fork()) < 0) {
		err_sys("fork error");
	} else if (child == 0) {	/* child copies stdin to ptym */
        for ( ; ; ) {
			if ((nread = read(STDIN_FILENO, buf, TBUFFSIZE)) < 0)
				err_sys("read error from stdin");
			else if (nread == 0)
				break;		/* EOF on stdin means we're done */
			if (writen(ptym, buf, nread) != nread)
				err_sys("writen error to master pty");
		}

		/*
		 * We always terminate when we encounter an EOF on stdin,
		 * but we notify the parent only if ignoreeof is 0.
		 */
		if (ignoreeof == 0)
			kill(getppid(), SIGTERM);	/* notify parent */
        exit(0);	/* and terminate; child can't return */
	}

	/*
	 * Parent copies ptym to stdout.
	 */
	if (signal_intr(SIGTERM, sig_term) == SIG_ERR)
		err_sys("signal_intr error for SIGTERM");

	for ( ; ; ) {
		if ((nread = read(ptym, buf, TBUFFSIZE)) <= 0)
			break;		/* signal caught, error, or EOF */
		if (writen(STDOUT_FILENO, buf, nread) != nread)
			err_sys("writen error to stdout");
	}

	/*
	 * There are three ways to get here: sig_term() below caught the
	 * SIGTERM from the child, we read an EOF on the pty master (which
	 * means we have to signal the child to stop), or an error.
	 */
	if (sigcaught == 0)	/* tell child if it didn't send us the signal */
		kill(child, SIGTERM);

	/*
	 * Parent returns to caller.
	 */
}

/*
 * The child sends us SIGTERM when it gets EOF on the pty slave or
 * when read() fails.  We probably interrupted the read() of ptym.
 */
static void sig_term(int signo)
{
	sigcaught = 1;		/* just set flag and return */

	printf("call ID: %d\n", getpid());
}