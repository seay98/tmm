include ../Make.defines

PROGS = serv01
OBJS = serv.o

all: ${PROGS}

cli01: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
	rm -f ${PROGS} ${OBJS}
