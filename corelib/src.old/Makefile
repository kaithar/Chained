
CC = gcc
CFLAGS = -export-dynamic -g -fPIC -I.
LIBS = -lm -lc -ldl -lssl
OBJS =  data-structures/fifo.o data-structures/linklist.o data-structures/map.o data-structures/persistent_map.o net/conn_basics.o net/ipv4_tcp.o net/openssl.o support/cores.o support/modules.o support/strings.o support/support.o config.o events.o main.o timers.o

# ----

all:	$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -shared -o libchained.so $(OBJS)

# ----

clean:
	rm -f *.so
	find -name \*.o -exec rm -v {} \;

# ----

install:
	cp libchained.so /usr/lib/libchained.so.0.0
	cp libchained.so /usr/lib/libchained.so.0
	cp libchained.so /usr/lib/libchained.so
	mkdir -p /usr/include/libchained/types
	cp libchained/*.h /usr/include/libchained/
	cp libchained/types/*.h /usr/include/libchained/types

data-structures/fifo.o: data-structures/fifo.c libchained/chained.h \
 libchained/config.h libchained/fifo.h libchained/linklist.h \
 libchained/map.h libchained/persistent_map.h \
 libchained/types/connection.h libchained/types/timers.h \
 libchained/types/socket_engine.h libchained/types/config_reading.h \
 libchained/types/modules.h libchained/main.h libchained/conn_basics.h \
 libchained/strings.h libchained/support.h libchained/socketengines.h \
 libchained/ipv4.h libchained/config_reading.h libchained/events.h \
 libchained/modules.h libchained/ssl.h libchained/timers.h

data-structures/linklist.o: data-structures/linklist.c \
 libchained/chained.h libchained/config.h libchained/fifo.h \
 libchained/linklist.h libchained/map.h libchained/persistent_map.h \
 libchained/types/connection.h libchained/types/timers.h \
 libchained/types/socket_engine.h libchained/types/config_reading.h \
 libchained/types/modules.h libchained/main.h libchained/conn_basics.h \
 libchained/strings.h libchained/support.h libchained/socketengines.h \
 libchained/ipv4.h libchained/config_reading.h libchained/events.h \
 libchained/modules.h libchained/ssl.h libchained/timers.h

data-structures/map.o: data-structures/map.c libchained/chained.h \
 libchained/config.h libchained/fifo.h libchained/linklist.h \
 libchained/map.h libchained/persistent_map.h \
 libchained/types/connection.h libchained/types/timers.h \
 libchained/types/socket_engine.h libchained/types/config_reading.h \
 libchained/types/modules.h libchained/main.h libchained/conn_basics.h \
 libchained/strings.h libchained/support.h libchained/socketengines.h \
 libchained/ipv4.h libchained/config_reading.h libchained/events.h \
 libchained/modules.h libchained/ssl.h libchained/timers.h \
 libchained/map_presets.h

data-structures/persistent_map.o: data-structures/persistent_map.c \
 libchained/chained.h libchained/config.h libchained/fifo.h \
 libchained/linklist.h libchained/map.h libchained/persistent_map.h \
 libchained/types/connection.h libchained/types/timers.h \
 libchained/types/socket_engine.h libchained/types/config_reading.h \
 libchained/types/modules.h libchained/main.h libchained/conn_basics.h \
 libchained/strings.h libchained/support.h libchained/socketengines.h \
 libchained/ipv4.h libchained/config_reading.h libchained/events.h \
 libchained/modules.h libchained/ssl.h libchained/timers.h

net/conn_basics.o: net/conn_basics.c libchained/chained.h \
 libchained/config.h libchained/fifo.h libchained/linklist.h \
 libchained/map.h libchained/persistent_map.h \
 libchained/types/connection.h libchained/types/timers.h \
 libchained/types/socket_engine.h libchained/types/config_reading.h \
 libchained/types/modules.h libchained/main.h libchained/conn_basics.h \
 libchained/strings.h libchained/support.h libchained/socketengines.h \
 libchained/ipv4.h libchained/config_reading.h libchained/events.h \
 libchained/modules.h libchained/ssl.h libchained/timers.h

net/ipv4_tcp.o: net/ipv4_tcp.c libchained/chained.h libchained/config.h \
 libchained/fifo.h libchained/linklist.h libchained/map.h \
 libchained/persistent_map.h libchained/types/connection.h \
 libchained/types/timers.h libchained/types/socket_engine.h \
 libchained/types/config_reading.h libchained/types/modules.h \
 libchained/main.h libchained/conn_basics.h libchained/strings.h \
 libchained/support.h libchained/socketengines.h libchained/ipv4.h \
 libchained/config_reading.h libchained/events.h libchained/modules.h \
 libchained/ssl.h libchained/timers.h

net/openssl.o: net/openssl.c libchained/chained.h libchained/config.h \
 libchained/fifo.h libchained/linklist.h libchained/map.h \
 libchained/persistent_map.h libchained/types/connection.h \
 libchained/types/timers.h libchained/types/socket_engine.h \
 libchained/types/config_reading.h libchained/types/modules.h \
 libchained/main.h libchained/conn_basics.h libchained/strings.h \
 libchained/support.h libchained/socketengines.h libchained/ipv4.h \
 libchained/config_reading.h libchained/events.h libchained/modules.h \
 libchained/ssl.h libchained/timers.h

support/cores.o: support/cores.c libchained/chained.h libchained/config.h \
 libchained/fifo.h libchained/linklist.h libchained/map.h \
 libchained/persistent_map.h libchained/types/connection.h \
 libchained/types/timers.h libchained/types/socket_engine.h \
 libchained/types/config_reading.h libchained/types/modules.h \
 libchained/main.h libchained/conn_basics.h libchained/strings.h \
 libchained/support.h libchained/socketengines.h libchained/ipv4.h \
 libchained/config_reading.h libchained/events.h libchained/modules.h \
 libchained/ssl.h libchained/timers.h

support/modules.o: support/modules.c libchained/chained.h \
 libchained/config.h libchained/fifo.h libchained/linklist.h \
 libchained/map.h libchained/persistent_map.h \
 libchained/types/connection.h libchained/types/timers.h \
 libchained/types/socket_engine.h libchained/types/config_reading.h \
 libchained/types/modules.h libchained/main.h libchained/conn_basics.h \
 libchained/strings.h libchained/support.h libchained/socketengines.h \
 libchained/ipv4.h libchained/config_reading.h libchained/events.h \
 libchained/modules.h libchained/ssl.h libchained/timers.h

support/strings.o: support/strings.c libchained/chained.h \
 libchained/config.h libchained/fifo.h libchained/linklist.h \
 libchained/map.h libchained/persistent_map.h \
 libchained/types/connection.h libchained/types/timers.h \
 libchained/types/socket_engine.h libchained/types/config_reading.h \
 libchained/types/modules.h libchained/main.h libchained/conn_basics.h \
 libchained/strings.h libchained/support.h libchained/socketengines.h \
 libchained/ipv4.h libchained/config_reading.h libchained/events.h \
 libchained/modules.h libchained/ssl.h libchained/timers.h

support/support.o: support/support.c libchained/chained.h \
 libchained/config.h libchained/fifo.h libchained/linklist.h \
 libchained/map.h libchained/persistent_map.h \
 libchained/types/connection.h libchained/types/timers.h \
 libchained/types/socket_engine.h libchained/types/config_reading.h \
 libchained/types/modules.h libchained/main.h libchained/conn_basics.h \
 libchained/strings.h libchained/support.h libchained/socketengines.h \
 libchained/ipv4.h libchained/config_reading.h libchained/events.h \
 libchained/modules.h libchained/ssl.h libchained/timers.h

config.o: config.c libchained/chained.h libchained/config.h \
 libchained/fifo.h libchained/linklist.h libchained/map.h \
 libchained/persistent_map.h libchained/types/connection.h \
 libchained/types/timers.h libchained/types/socket_engine.h \
 libchained/types/config_reading.h libchained/types/modules.h \
 libchained/main.h libchained/conn_basics.h libchained/strings.h \
 libchained/support.h libchained/socketengines.h libchained/ipv4.h \
 libchained/config_reading.h libchained/events.h libchained/modules.h \
 libchained/ssl.h libchained/timers.h

events.o: events.c libchained/chained.h libchained/config.h \
 libchained/fifo.h libchained/linklist.h libchained/map.h \
 libchained/persistent_map.h libchained/types/connection.h \
 libchained/types/timers.h libchained/types/socket_engine.h \
 libchained/types/config_reading.h libchained/types/modules.h \
 libchained/main.h libchained/conn_basics.h libchained/strings.h \
 libchained/support.h libchained/socketengines.h libchained/ipv4.h \
 libchained/config_reading.h libchained/events.h libchained/modules.h \
 libchained/ssl.h libchained/timers.h

main.o: main.c libchained/chained.h libchained/config.h libchained/fifo.h \
 libchained/linklist.h libchained/map.h libchained/persistent_map.h \
 libchained/types/connection.h libchained/types/timers.h \
 libchained/types/socket_engine.h libchained/types/config_reading.h \
 libchained/types/modules.h libchained/main.h libchained/conn_basics.h \
 libchained/strings.h libchained/support.h libchained/socketengines.h \
 libchained/ipv4.h libchained/config_reading.h libchained/events.h \
 libchained/modules.h libchained/ssl.h libchained/timers.h

timers.o: timers.c libchained/chained.h libchained/config.h \
 libchained/fifo.h libchained/linklist.h libchained/map.h \
 libchained/persistent_map.h libchained/types/connection.h \
 libchained/types/timers.h libchained/types/socket_engine.h \
 libchained/types/config_reading.h libchained/types/modules.h \
 libchained/main.h libchained/conn_basics.h libchained/strings.h \
 libchained/support.h libchained/socketengines.h libchained/ipv4.h \
 libchained/config_reading.h libchained/events.h libchained/modules.h \
 libchained/ssl.h libchained/timers.h

