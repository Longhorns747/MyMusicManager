#ifndef NETWORKING_UTIL_H
#define NETWORKING_UTIL_H

#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <string.h>
#include "tpl.h"
#include "data_structs.h"

#define PORT 2222
#define BUFSIZE 1500
#define MESSAGESIZE sizeof(message)

typedef struct sockaddr_in sockaddr_in;

int setup_connection(sockaddr_in* address);
void setup_addr(char* IPaddr, sockaddr_in *address);
void send_payload(message* message, byte* payload, int sock);
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

void send_message(message* message, int sock)
{
    byte* buffer;
    size_t len;

    tpl_node *tn;
    tn = tpl_map("S(iiii)", message);
    tpl_pack(tn, 0);
    tpl_dump(tn, TPL_MEM, &buffer, &len);
    tpl_free(tn);

    send(sock, buffer, len, 0);
    free(buffer);
}

void send_payload(message* message, byte* payload, int sock)
{
    //Send the metadata
    if(send(sock, &message, MESSAGESIZE, 0) != MESSAGESIZE)
        perror("send() sent unexpected number of bytes for metadata");	

    byte buffer[BUFSIZE];

    //Send the payload 1500 (BUFSIZE) bytes at a time
    int remainingBytes = message->num_bytes;
    int offset = 0;
    int numSent;

    while(remainingBytes > BUFSIZE){
        numSent = send(sock, &payload[BUFSIZE*offset], BUFSIZE, 0);

	    if(numSent < 0)
	        perror("send() failed");

	    remainingBytes = remainingBytes - numSent;
	    offset=offset+1;				
    }

    //Send the remainder of the payload
    if(send(sock, &payload[BUFSIZE*offset], remainingBytes, 0) != remainingBytes)
	   perror("send() sent unexpected number of bytes of data");	
    
}

void rcv_message(message* message, int clientSock)
{
    byte rcvBuf[BUFSIZE];

    //Recieve metadata from client
    ssize_t bytesRecieved = recv(clientSock, rcvBuf, BUFSIZE, 0);

    if(bytesRecieved < sizeof(message)){
        printf("Did not recieve all of the message\n");
        exit(1);
    }

    tpl_node *tn;
    tn = tpl_map("S(iiii)", &message);
    tpl_load(tn, TPL_MEM, rcvBuf, sizeof(message));
    tpl_unpack(tn, 0);
    tpl_free(tn);

    //Remake the message struct
    /*memcpy(message, rcvBuf, sizeof(msg));

    int msgSize = msg.size;
    if(!msg.last)
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

    memcpy(message, messageBuf, msgSize);*/
}

#endif
