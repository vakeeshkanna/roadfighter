# A test Makefile

GPP = g++
FLAGS = -Wall
OBJS = list.o
INCDIR = ../include

list.o: list.cpp 
	${GPP}  -I${INCDIR} ${FLAGS} -c $< 

listdriver: listdriver.cpp ${OBJS}
# 	${GPP} ${FLAGS} -o $@ listdriver.cpp ${OBJS} 
	${GPP} -c listdriver.cpp
clean:
	rm -f *.o
