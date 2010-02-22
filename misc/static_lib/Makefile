# A test Makefile

GPP = g++
CFLAGS = -Wall
OBJS = ctest.o
AR = ar 
AFLAGS = -cvq
RL = ranlib

libdriver : libctest.a	  
	${GPP} ${CFLAGS} -o $@ libdriver.c -L. -lctest 

libctest.a : ctest.o
	rm -f libctest.a
	${AR} ${AFLAGS} $@ ${OBJS}
	${RL} $@	

ctest.o : ctest.c	
	${GPP} -c $< 

clean:
	rm -f *.o

#Using libtool to do the job.

libctest.la : ctest.c
	rm -f $@ 
	libtool -static -o $@ ${OBJS}
	mv $@ libctest.a
