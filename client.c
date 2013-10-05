#include <string.h>
#include "data_structs.h"
#include "file_util.h"
#include "networking_util.h"

int user_prompt();
void create_message(message* msg, int numBytes, int msgType);
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
		message msg;

		//payload will be a filestate in this case 
		filestate state;
		update_files(&state);
		
		//Serialize the filestate
		int numBytes = 3;
		create_message(&msg, numBytes, userChoice);
		printf("Created message with type: %d\n", msg.type);

		send_message(&msg, sock);

		//keep accepting metadata packets after a reponse until a last packet flag
		/*while(!lastPacket){
			//receive metadata response
			message* metadata;
			recv(sock, metadata, MESSAGESIZE, 0);
	
			int numBytes = metadata->num_bytes;
			lastPacket = metadata->last_message;
			message_type type = metadata->type;
				
			//accept mp3 file data
			if(type == PULL)
				pull();
			else if(type == LIST || DIFF)
			    list();
			else //type == LEAVE
			    printf("Now exiting.\n");
		}*/
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

void create_message(message* msg, int numBytes, int msgType)
{
	msg->num_bytes = numBytes;
	msg->filename_length = 0;
	msg->type = msgType;
	msg->last_message = 1;
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

void list()
{
	char* filename;
    //TODO: call recv, and use that buffer to print out each string name.
    //strings will be separated by /0
}