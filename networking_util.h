#ifndef NETWORKING_UTIL_H
#define NETWORKING_UTIL_H

#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <string.h>

#define PORT 2222
typedef struct sockaddr_in sockaddr_in;

int setup_connection(sockaddr_in* address);
void setup_addr(char* IPaddr, sockaddr_in *address);
void send_message(message* msg, int sock);

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
    int msgLength;

    //send msg to server 
    msgLength = sizeof(*message);
    
    if(send(sock, message, msgLength, 0) != msgLength)
        perror("send() sent unexpected number of bytes");
}

#endif