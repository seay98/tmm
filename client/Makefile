include ../Make.defines

PROGS = cli01 gaddr
OBJS = cli01.o cli_proc.o sys_info.o com_fuc.o pty_fuc.o pty.o pty_shell.o hip.o readn.o writen.o

all: ${PROGS}

cli01: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

gaddr: gen_addr.o
	${CC} ${CFLAGS} -o $@ gen_addr.o com_fuc.o ${LIBS}

clean:
	rm -f ${PROGS} ${OBJS} *.o
