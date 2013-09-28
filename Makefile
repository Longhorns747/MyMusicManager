CC=gcc

OS := $(shell uname -s)

# Extra LDFLAGS if Solaris
ifeq ($(OS), SunOS)
	LDFLAGS=-lsocket -lnsl
    endif

all: client server 

client: client.c
	$(CC) client.c -o musicManager -std=c99 

server: server.c
	$(CC) server.c -o musicServer -std=c99 

clean:
	    rm -f client server *.o
