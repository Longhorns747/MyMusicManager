#ifndef DATA_STRUCTS_H_INCLUDED
#define DATA_STRUCTS_H_INCLUDED

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		/* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		/* for sockaddr_in and inet_addr() */
#include <stdlib.h>

typedef enum {
	LIST, PULL, DIFF, LEAVE
} message_type;

typedef struct 
{
	int ID;
	char* filename;
	int num_bytes;
	unsigned char payload[num_bytes];
} Music_File;

typedef struct
{
	int music_ids[];
	//Not sure how to represent host here... int for IP? Char[]?
} Filestate;

typedef struct
{
	message_type type;
	Filestate state;
} Message;



#endif
