#ifndef NETWORKING_UTIL_H
#define NETWORKING_UTIL_H

#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <string.h>
#include "data_structs.h"

#define PORT 2222
#define RCVBUFSIZE 1500

typedef struct sockaddr_in sockaddr_in;

int setup_connection(sockaddr_in* address);
void setup_addr(char* IPaddr, sockaddr_in *address);
void send_message(message* msg, int sock);
void rcv_message(message* message, int sock);

//Set up the address structure for a given address
void setup_addr(char* IPaddr, sockaddr_in *address)
{
	memset(address, 0, sizeof(*address));

	//Setting the protocol family
    address->sin_family = AF_INET;

    //Formatting and setting the IP address
    int rtnVal = inet_pton(AF_INET, IPaddr, &address->sin_addr.s_addr);
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

    //Prepare metadata
    metadata md;
    int msgLength;
    
    msgLength = sizeof(*message);
    md.size = msgLength;
    md.last = true;

    int mdlength;
    mdlength = sizeof(md);

    //Send the metadata
    if(send(sock, &md, mdlength, 0) != mdlength)
        perror("send() sent unexpected number of bytes for metadata");
    
    //Send msg
    if(send(sock, message, msgLength, 0) != msgLength)
        perror("send() sent unexpected number of bytes for message");
}

void rcv_message(message* message, int sock)
{
    sockaddr_in clientAddr;
    unsigned int clntLen;
    int clientSock;
    byte rcvBuf[RCVBUFSIZE];

    /* Accept incoming connection */
    clntLen = sizeof(clientAddr);
    clientSock = accept(sock, (struct sockaddr*) &clientAddr, &clntLen);

    if(clientSock < 0){
        printf("accept() failed :(\n");
        exit(1);
    }

    printf("Client accepted... \n");

    //Recieve metadata from client
    ssize_t bytesRecieved = recv(clientSock, rcvBuf, RCVBUFSIZE, 0);

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
    
    memcpy(message, messageBuf, msgSize);
}

#endif