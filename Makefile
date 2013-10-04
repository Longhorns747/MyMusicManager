CC=gcc

OS := $(shell uname -s)

# Extra LDFLAGS if Solaris
ifeq ($(OS), SunOS)
	LDFLAGS=-lsocket -lnsl
    endif

all: client server 

client: client.c
	$(CC) tpl.c client.c -o musicManager -std=c99 -lcrypto

server: server.c
	$(CC) tpl.c server.c -o musicServer -std=c99 -lcrypto

clean:
	    rm -f client server *.o
