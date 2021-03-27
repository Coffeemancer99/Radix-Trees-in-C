CPP = gcc
FLAGS = -Wall -std=c99 -pedantic -g

EXEC = radixtrees
OBJS = radixMain.o radixFunc.o

default:${EXEC}

clean:
	rm -f ${EXEC}
	rm -f *.o

run: ${EXEC}
	./${EXEC}

${EXEC}:${OBJS}
	${CPP} ${FLAGS} -o ${EXEC} ${OBJS}

.c.o:
	${CPP} ${FLAGS} -c $<

radixMain.o: radixMain.c radHeader.h
radixFunc.o: radixFunc.c radHeader.h
