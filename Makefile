LDLIBS = -pthread
CFLAGS = -I./includes
SOURCES = src/pclient.c src/threadpool.c
.PHONY: clean

client:

server: $(SOURCES:.c=.o)

clean:
	rm server client src/*.o
