include ../Make.defines

PROGS = cli01
OBJS = cli01.o cli_proc.o sys_info.o com_fuc.o

all: ${PROGS}

cli01: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
	rm -f ${PROGS} ${OBJS}
