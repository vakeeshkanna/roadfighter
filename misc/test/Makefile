# A test Makefile

GPP = g++
FLAGS = -Wall
OBJS = abc.o
INCDIR = ../../include

test :	${OBJS}  
	${CC} ${FLAGS} -o $@ test.c ${OBJS}

abc.o : abc.c	
	${CC} -c $< 

clean:
	rm -f *.o

driver : driver.cpp copy.o
	${GPP} ${FLAGS} -I${INCDIR} -o $@ $< copy.o

copy.o : copy.cpp 
	${GPP} -c $< -I${INCDIR}
