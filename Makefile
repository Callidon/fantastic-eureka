LDLIBS = -pthread -lncurses
CFLAGS = -I./includes
SOURCES = src/handler.c src/array_client.c src/decoder.c
.PHONY: clean

client: $(SOURCES:.c=.o)

server: $(SOURCES:.c=.o)

clean:
	-rm server client src/*.o
