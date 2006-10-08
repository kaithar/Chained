CC = gcc
CFLAGS = -export-dynamic -g -fPIC -pipe -I../includes/
LIBS = -lm -lc -ldl

OBJS = conn_basics.o main.o strings.o linklist.o map.o se_select.o

INCS = ../includes/chained.h

all: $(OBJS)
	gcc $(LIBS) $(CFLAGS) -shared -o libchained.so $(OBJS)

clean:
	rm -f *.o libchained.so;
