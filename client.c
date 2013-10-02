#include <string.h>
#include "data_structs.h"
#include "file_util.h"
#include "networking_util.h"

const char selections[4][5] = {"LEAVE", "LIST", "PULL", "DIFF"};

int user_prompt();
void create_message(message* msg, int msgType);

int main()
{
	int userChoice = 1;

	//Set up a connection to the server
	sockaddr_in serverAddr;
	setup_addr("130.207.114.21", &serverAddr);
	int sock = setup_connection(&serverAddr);

	printf("Connection to server established\n");
	printf("Welcome to your Music Manager :D!\n");

	int msgLength;
	while(userChoice)
	{
		userChoice = user_prompt();
		message* msg;
		create_message(msg, userChoice);
		printf("Created message with type: %d file 3: %s\n", msg->type, msg->state->music_files[2].filename);

		//send msg to server 
		msgLength = sizeof(message);
	        if(send(sock, msg, msgLength, 0) != msgLength)
		    perror("send() sent unexpected number of bytes");

		//receive response
		//This should be in a loop. How do we know when to stop receiving data?
		//int numBytes = recv(sock, rcvBuf, RCVBUFSIZE, 0);
	
		


		free_files(msg->state);
	}
	
	return 0;
}

int user_prompt()
{
	char selection[5];
	printf("Please make a selection from one of the following: LEAVE, LIST, PULL, DIFF\n");
	scanf("%s", selection);

	//***
	//What happens on bad user input? -SC
        //***

	int select = 0;

	for(int i = 0; i < NUM_MESSAGES; i++){
		if(!strcmp(selection, selections[i])){
			select = i;
		}
	}

	return select;
}

void create_message(message* msg, int msgType)
{
	filestate currState;
	int numFiles = update_files(&currState);

	msg->state = &currState;
	msg->type = (message_type) msgType;
}
