#ifndef DATA_STRUCTS_H_INCLUDED
#define DATA_STRUCTS_H_INCLUDED

#include <stdio.h>		    /* for printf() and fprintf() */
#include <stdlib.h>
#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <stdbool.h>

int NUM_MESSAGES = 4;

typedef enum {
	LEAVE, LIST, PULL, DIFF
} message_type;

typedef struct
{
	bool last;
	int size;
} metadata;

typedef struct 
{
	char* filename;
	unsigned char* ID;
} music_file;

typedef struct
{
	struct sockaddr_in hostIP;
	int numFiles;
	music_file* music_files;
} filestate;

typedef struct
{
	message_type type;
	filestate* state;
} message;

#endif
