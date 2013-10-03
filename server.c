#include <sys/stat.h>
#include "data_structs.h"
#include "file_util.h"
#include "networking_util.h"

#define MAX_PENDING 10

void setup_serveraddr(sockaddr_in* serverAddr);
void make_socket(int* sock);

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

    while(1){
        message* msg;
        rcv_message(msg, sock);

        if(*msg == NULL)
            continue;
        else
            printf("Whoa a message! Type: %d\n", msg->type);
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
    serverAddr->sin_port = PORT;
}