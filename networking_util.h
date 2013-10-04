#ifndef NETWORKING_UTIL_H
#define NETWORKING_UTIL_H

#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <string.h>
#include "data_structs.h"

#define PORT 2222
#define BUFSIZE 1500
#define METADATASIZE 16

typedef struct sockaddr_in sockaddr_in;

int setup_connection(sockaddr_in* address);
void setup_addr(char* IPaddr, sockaddr_in *address);
void send_message(message* message, byte* payload, int sock);
void rcv_message(message* message, int sock);

//Set up the address structure for a given address
void setup_addr(char* IPaddr, sockaddr_in *address)
{
	memset(address, 0, sizeof(*address));

	//Setting the protocol family
    address->sin_family = AF_INET;

    //Formatting and setting the IP address
    int rtnVal = inet_pton(AF_INET, IPaddr, &(address->sin_addr.s_addr));
    if(rtnVal <= 0){
        printf("inet_pton failed :(\n");
        exit(1);
    }

    //Setting the port
    address->sin_port = htons(PORT);
}

//Returns a socket to setup the connection with a host
int setup_connection(sockaddr_in* address)
{
    /* Create a new TCP socket*/
    int clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSock < 0){
        printf("socket() failed :(\n");
        exit(1);
    }

    if(connect(clientSock, (struct sockaddr *) address, sizeof(*address)) < 0){
        printf("connect() failed :(\n");
        exit(1);
    }

    return clientSock;
}

void send_message(message* message, byte* payload, int sock)
{

    byte metadata_buffer[METADATASIZE];	
    
    //memcpy(buffer[0], (void*)message->type, sizeof(int));
    //memcpy(buffer[4], (void*)message->num_bytes, sizeof(int));
    //memcpy(buffer[8], (void*)message->last_message, sizeof(int)); 	
    //memcpy(buffer[12], (void*)message->filename_length, sizeof(int)); 	

    //Pack the metadata buffer
    //metadata_buffer[0] = message->type;
    //metadata_buffer[4] = message->num_bytes;
    //metadata_buffer[8] = message->last_message;
    //metadata_buffer[12] = message->filename_length;


    //Send the metadata
    if(send(sock, &message, METADATASIZE, 0) != METADATASIZE)
        perror("send() sent unexpected number of bytes for metadata");	

    byte buffer[BUFSIZE];	
    //Send the payload 1500 (BUFSIZE) bytes at a time
    int remainingBytes = message->num_bytes;
    int offset = 0;
    while(remainingBytes > BUFSIZE){
	if(send(sock, payload[BUFSIZE*offset], BUFSIZE, 0) != BUFSIZE)
	    perror("send() sent unexpected number of bytes of data");
	remainingBytes = remainingBytes - BUFSIZE;
	offset=offset+1;				
    }

    //Send the remainder of the payload
    if(send(sock, payload[BUFSIZE*offset], remainingBytes, 0) != remainingBytes)
	perror("send() sent unexpected number of bytes of data");	
    
}

void rcv_message(message* message, int clientSock)
{
    byte rcvBuf[BUFSIZE];

    //Recieve metadata from client
    ssize_t bytesRecieved = recv(clientSock, rcvBuf, BUFSIZE, 0);

    if(bytesRecieved < sizeof(metadata)){
        printf("Did not recieve all of metadata\n");
        exit(1);
    }




    //Remake the metadata struct
    metadata md;
    memcpy(&md, rcvBuf, sizeof(md));

    int msgSize = md.size;
    if(!md.last)
    {
        printf("Metadata does not have last set... What were you thinking???");
        exit(1);
    }

    //Let's recieve the message!
    byte messageBuf[msgSize];
    int totalBytes = 0;

    while(totalBytes < msgSize){
        bytesRecieved = recv(clientSock, messageBuf, msgSize, 0);
        totalBytes += bytesRecieved;
    }
    
    printf(":|\n");
    memcpy(message, messageBuf, msgSize);
    printf(":3\n");
}

#endif
