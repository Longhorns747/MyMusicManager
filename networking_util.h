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
void send_filenames(filestate* state, int sock);
void rcv_filenames(int sock);
void send_ids(filestate* state, int sock);

void create_message(message* msg, int numBytes, int msgType, int last_message, int filename_length)
{
    msg->num_bytes = numBytes;
    msg->filename_length = filename_length;
    msg->type = msgType;
    msg->last_message = last_message;
}

void send_message(message* msg, int sock)
{
    ssize_t len = sizeof(message);

    send(sock, msg, len, 0);
}

void rcv_message(message* msg, int sock)
{
    byte rcvBuf[sizeof(message)];

    //Recieve metadata from client
    ssize_t bytesRecieved;
    int totalBytes = 0;

    while(totalBytes < sizeof(message)){
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

void send_filenames(filestate* state, int sock)
{
    for(int i = 0; i < state->numFiles; i++){
        char* filename = state->music_files[i].filename;

        message msg;
        create_message(&msg, strlen(filename), LIST, 0, strlen(filename));

        send_payload(&msg, filename, sock);
    }

    //Make the last message
    message lastMsg;
    create_message(&lastMsg, 0, LIST, 1, 0);
    send_message(&lastMsg, sock);
}

void rcv_filenames(int sock)
{
    printf("Waiting for a list of files...\n");
    message msg;
    rcv_message(&msg, sock);
    int count = 1;

    while(!msg.last_message)
    {
        //Recieve filename from server
        byte filename[msg.num_bytes + 1];
        memset(filename, 0, msg.num_bytes);

        ssize_t bytesRecieved;
        int totalBytes = 0;

        while(totalBytes < msg.num_bytes){
            bytesRecieved = recv(sock, filename, msg.num_bytes, 0);
            if(bytesRecieved < 0)
                printf("Couldn't recieve filenames :(");

            totalBytes += bytesRecieved;
        }

        filename[msg.num_bytes] = '\0';
        printf("File %d: %s\n", count, (char *)filename);
        count++;

        memset(filename, 0, msg.num_bytes);
        rcv_message(&msg, sock);
    }
}

void send_ids(filestate* state, int sock)
{
    for(int i = 0; i < state->numFiles; i++){
        char* ID = state->music_files[i].ID;

        message msg;
        create_message(&msg, strlen(ID), -1, 0, strlen(ID));

        send_payload(&msg, ID, sock);
    }

    //Make the last message
    message lastMsg;
    create_message(&lastMsg, 0, -1, 1, 0);
    send_message(&lastMsg, sock);
}

void rcv_IDs(filestate* res, int sock)
{
    printf("Waiting for file IDs...\n");
    message msg;
    rcv_message(&msg, sock);
    int count = 0;

    music_file* fileList;
    fileList = (music_file*) malloc(sizeof(music_file));

    while(!msg.last_message)
    {
        fileList = (music_file*) realloc(fileList, sizeof(music_file)*(count + 1)); 

        //Recieve ID from server
        byte ID[msg.num_bytes + 1];

        ssize_t bytesRecieved;
        int totalBytes = 0;

        while(totalBytes < msg.num_bytes){
            bytesRecieved = recv(sock, ID, msg.num_bytes, 0);

            if(bytesRecieved < 0)
                printf("Couldn't recieve ID :(");

            totalBytes += bytesRecieved;
        }

        music_file* currFile = (music_file*) malloc(sizeof(music_file));
        currFile->ID = ID;
        fileList[count] = *currFile;

        count++;
        rcv_message(&msg, sock);
    }

    //This doesn't seem to be working...
    res->music_files = fileList;
    res->numFiles = count;
}

#endif
