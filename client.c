#include <string.h>
#include "data_structs.h"
#include "file_util.h"
#include "networking_util.h"

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

	while(userChoice)
	{
		userChoice = user_prompt();
		message* msg;
		create_message(msg, userChoice);
		printf("Created message with type: %s\n", selections[msg->type]);
		
		send_message(msg, sock);

		//receive response
		//This should be in a loop. How do we know when to stop receiving data?
		//int numBytes = recv(sock, rcvBuf, RCVBUFSIZE, 0);

		//free_files(msg->state);

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
	msg->filename_length = 0; //Not used by client
	msg->payload = &currState; //This should be a pointer to payload. Is this right?
	msg->type = (message_type) msgType;

}
