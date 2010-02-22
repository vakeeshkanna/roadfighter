# A test Makefile

GPP = g++
CFLAGS = -Wall
OBJS = ctest.o
USRLIB =/usr/lib/

#Don't need as default on Darwin
LIBFLAGS = -fPIC

SHAREDFLAGS = -shared -Wl, -soname,

libdriver : libctest.so	  
	${GPP} ${CFLAGS} -o $@ libdriver.c  ${USRLIB}libctest.so #-lctest 

libctest.so : ${OBJS} 
	libtool -dynamic -o libctest.so ${OBJS}
	mv $@ ${USRLIB}

#	${CC} ${SHAREDFLAGS} libctest.so. -o libctest.so.1.0 ${OBJS} 
#	mv libctest.so.1.0 /usr/lib
#	ln -sf /usr/lib/libctest.so.1.0 /usr/lib/libctest.so
#	ln -sf /usr/lib/libctest.so.1.0 /usr/lib/libctest.so.1 

libdriverdynamic : libctest.so
	${GPP} -o $@ libdriverdynamic.c
ctest.o : ctest.c	
	${GPP} ${CFLAGS} -c  $< 
clean:
	rm -f *.o
