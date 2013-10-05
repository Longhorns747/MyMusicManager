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

void create_message(message* msg, int numBytes, int msgType, int last_message, int filename_length);
void send_message(message* msg, int sock);
void send_payload(message* msg, byte* payload, int sock);
void rcv_message(message* msg, int sock);

void create_message(message* msg, int numBytes, int msgType, int last_message, int filename_length)
{
    msg->num_bytes = numBytes;
    msg->filename_length = filename_length;
    msg->type = msgType;
    msg->last_message = last_message;
}

void send_message(message* msg, int sock)
{
    ssize_t len = sizeof(msg);

    send(sock, msg, len, 0);
}

void rcv_message(message* msg, int sock)
{
    byte rcvBuf[sizeof(message)];

    //Recieve metadata from client
    ssize_t bytesRecieved;
    int totalBytes = 0;

    while(totalBytes < sizeof(msg)){
        bytesRecieved = recv(sock, rcvBuf, sizeof(message), 0);

        if(bytesRecieved < 0)
            perror("Recv failed :(");

        totalBytes += bytesRecieved;
    }

    memcpy(msg, rcvBuf, sizeof(message));
}

void send_payload(message* msg, byte* payload, int sock)
{
    //Send the metadata
    if(send(sock, msg, MESSAGESIZE, 0) != MESSAGESIZE)
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
    if(send(sock, payload + (BUFSIZE*offset), remainingBytes, 0) != remainingBytes)
       perror("send() sent unexpected number of bytes of data");       
}

#endif
