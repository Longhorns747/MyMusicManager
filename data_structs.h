#ifndef DATA_STRUCTS_H_INCLUDED
#define DATA_STRUCTS_H_INCLUDED

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		/* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		/* for sockaddr_in and inet_addr() */
#include <stdlib.h>

int NUM_MESSAGES = 4;

typedef enum {
	LEAVE, LIST, PULL, DIFF
} message_type;

typedef struct 
{
	int ID;
	char* filename;
	int num_bytes;
	unsigned char payload[];
} Music_File;

typedef struct
{
	struct sockaddr_in clinetIP;
	int music_ids[];
} Filestate;

typedef struct
{
	message_type type;
	Filestate state;
} Message;

#endif
