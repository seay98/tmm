include ../Make.defines

PROGS = cli01
OBJS = cli01.o cli_proc.o sys_info.o com_fuc.o driver.o pty_fuc.o pty.o

all: ${PROGS}

cli01: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
	rm -f ${PROGS} ${OBJS}
