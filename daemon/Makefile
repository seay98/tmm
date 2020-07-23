include ../Make.defines

PROGS = clid
OBJS = clid.o init.o single.o lockfile.o hdown.o

all: ${PROGS}

clid: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} ${LIBS}

clean:
	rm -f ${PROGS} ${OBJS} *.o
