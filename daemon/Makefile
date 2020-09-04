include ../Make.defines

PROGS = clid hclid
OBJS = init.o single.o lockfile.o hdown.o ../lib/com_fuc.o hpost.o sys_info.o base64.o

all: ${PROGS}

clid: ${OBJS} clid.o
	${CC} ${CFLAGS} -o $@ ${OBJS} clid.o ${LIBS}

hclid: ${OBJS} hclid.o hgetip.o
	${CC} ${CFLAGS} -o $@ ${OBJS} hclid.o hgetip.o ${LIBS}

clean:
	rm -f ${PROGS} ${OBJS} *.o
