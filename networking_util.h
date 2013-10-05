#ifndef NETWORKING_UTIL_H
#define NETWORKING_UTIL_H

#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <string.h>
#include "data_structs.h"

#define PORT 2222
#define BUFSIZE 1500
#define MESSAGESIZE sizeof(message)

typedef struct sockaddr_in sockaddr_in;

int setup_connection(sockaddr_in* address);
void setup_addr(char* IPaddr, sockaddr_in *address);
void send_message(message* msg, int sock);
void send_payload(message* msg, byte* payload, int sock);
void rcv_message(message* msg, int sock);

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

void send_message(message* msg, int sock)
{
    ssize_t len = sizeof(msg);

    send(sock, msg, len, 0);
}

void rcv_message(message* msg, int clientSock)
{
    byte rcvBuf[sizeof(msg)];

    //Recieve metadata from client
    ssize_t bytesRecieved;
    int totalBytes = 0;

    while(totalBytes < sizeof(msg)){
        bytesRecieved = recv(clientSock, rcvBuf, sizeof(msg), 0);

        if(bytesRecieved < 0)
            perror("Recv failed :(");

        totalBytes += bytesRecieved;
    }

    memcpy(msg, rcvBuf, sizeof(message));
}

void send_payload(message* msg, byte* payload, int sock)
{
    //Send the metadata
    if(send(sock, &msg, MESSAGESIZE, 0) != MESSAGESIZE)
        perror("send() sent unexpected number of bytes for metadata");  

    byte buffer[BUFSIZE];

    //Send the payload 1500 (BUFSIZE) bytes at a time
    int remainingBytes = msg->num_bytes;
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

#endif
