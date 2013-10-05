#include <string.h>
#include "data_structs.h"
#include "file_util.h"
#include "networking_util.h"

int setup_connection(sockaddr_in* address);
void setup_addr(char* IPaddr, sockaddr_in *address);
int user_prompt();
void pull();
void list();

int main()
{
	int userChoice = 1;

	//Set up a connection to the server
	sockaddr_in serverAddr;
	setup_addr("130.207.114.21", &serverAddr);
	int sock = setup_connection(&serverAddr);

	printf("Connection to server established\n");
	printf("Welcome to your Music Manager :D!\n");

	int lastPacket = 1;

	while(userChoice)
	{
		userChoice = user_prompt();

		//Handle bad user input
		if(userChoice == -1)
			continue;

		message msg;
		
		int numBytes = 3;
		create_message(&msg, numBytes, userChoice, 1, 1);
		send_message(&msg, sock);

		switch(msg.type){
            case LEAVE:
                exit(1);
            case LIST:
                list(sock);
        }
	}

	return 0;
}

int user_prompt()
{
	char selection[5];
	printf("Please make a selection from one of the following: LEAVE, LIST, PULL, DIFF\n");
	scanf("%s", selection);

	int select = -1;

	for(int i = 0; i < NUM_MESSAGES; i++){
		if(!strcmp(selection, selections[i])){
			select = i;
		}
	}

	return select;
}

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

void pull(int numBytes, int sock)
{
	//TODO: handle filename. FIlename should be at the beginning of each mp3 file payload 	
    char file[numBytes]; //maybe make this a file type?
    int numBytesRecv = 0;
    byte rcvMsg;

    while(numBytesRecv < (numBytes-BUFSIZE)){
		recv(sock, &rcvMsg, BUFSIZE, 0);
		//memcpy(file[numBytesRecv], rcvMsg, BUFSIZE);
    }

    //grab the rest of the bytes
    recv(sock, &rcvMsg, BUFSIZE - numBytesRecv, 0);
    //memcpy(file[numBytesRecv], rcvMsg, BUFSIZE - numBytesRecv);

    //at this point we have the entire music file. Store it in memory somehow
}

void list(int sock)
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