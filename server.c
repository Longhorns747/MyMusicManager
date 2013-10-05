#include <sys/stat.h>
#include "data_structs.h"
#include "file_util.h"
#include "networking_util.h"

#define MAX_PENDING 10

void setup_serveraddr(sockaddr_in* serverAddr);
void make_socket(int* sock);
void list(int sock);
void leave(int sock);
void diff(int sock);

int main()
{

    int sock;
    make_socket(&sock);

    sockaddr_in serverAddr;
    setup_serveraddr(&serverAddr);

    /* Bind to local address structure */
    if(bind(sock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        printf("bind() failed :(\n");
        exit(1);
    }

    /* Listen for incoming connections */
    if(listen(sock, MAX_PENDING)){
        printf("listen() failed :(\n");
        exit(1);
    }

    /* Accept incoming connection */
    sockaddr_in clientAddr;
    unsigned int clntLen;
    int clientSock;

    clntLen = sizeof(clientAddr);
    clientSock = accept(sock, (struct sockaddr*) &clientAddr, &clntLen);

    if(clientSock < 0){
        printf("accept() failed :(\n");
        exit(1);
    }

    printf("Client accepted... \n");

    //Recieve and handle messages
    while(1){
        message msg;
        printf("Ready to recieve messages!\n");
        
        rcv_message(&msg, clientSock);
        printf("Whoa a message! Type: %d\n", msg.type);
        if(msg.type == -1)
            continue;

        switch(msg.type){
            case LEAVE:
                leave(clientSock);
                break;
            case LIST:
                list(clientSock);
                break;
            case DIFF:
                diff(clientSock);
                break;
        }

    }
    
    return 0;
}

void make_socket(int* sock)
{
    /* Create new TCP Socket for incoming requests*/
    in_port_t serverPort = htons(PORT);
    if((*sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        printf("socket() failed :(\n");
    }

    //Set socket options so that we can release the port
    int on = 1;
    setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
}

void setup_serveraddr(sockaddr_in* serverAddr)
{
    /* Construct local address structure*/
    memset(serverAddr, 0, sizeof(*serverAddr));

    //Protocol family, address, port
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr->sin_port = htons(PORT);
}

void list(int sock)
{
    printf("Doing a LIST :O\n");

    //Get current filestate
    filestate currState;
    update_files(&currState);
    send_filenames(&currState, sock);
}

void leave(int sock)
{
    printf("Doing a LEAVE :O\n");
    close(sock);
}

void diff(int sock)
{
    printf("Doing a DIFF :O\n");
    filestate senderIDs;
    filestate currState;
    filestate diff;
    //rcv_IDs(&senderIDs, sock);


    update_files(&currState);
    //delta(&currState, &senderIDs, &diff);
    //send_filenames(&diff, sock);
}