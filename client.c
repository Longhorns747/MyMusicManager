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

	int lastPacket = 1;
	while(userChoice)
	{
		userChoice = user_prompt();
		message* msg;
		byte* payload = create_message(msg, userChoice);
		printf("Created message with type: %s\n", selections[msg->type]);
		
		send_message(&msg, &payload, sock);
		//free_files(msg);

		//keep accepting metadata packets after a reponse until a last packet flag
		while(!lastPacket){
			//receive metadata response
			message* metadata;
			int whatsAGoodNameForThis = recv(sock, metadata, METADATASIZE, 0);
	
			int numBytes = metadata->num_bytes;
			message_type type = metadata->type;
			lastPacket = metadata->last_packet;

			//accept mp3 file data
			if(type == DIFF){

			    //TODO: handle filename. FIlename should be at the beginning of each mp3 file payload 	
			    file[numBytes];
			    int numBytesRecv = 0;
			    while(numBytesRecv < (numBytes-BUFSIZE){
				recv(sock, rcvMsg, BUFSIZE, 0);
				memcopy(file[numBytesRecv], rcvMsg, BUFSIZE);
			    }
			    //grab the rest of the bytes
			    recv(sock, rcvMsg, BUFSIZE - numBytesRecv, 0);
			    memcopy(file[numBytesRecv], rcvMsg, BUFSIZE - numBytesRecv);
			
			    //at this point we have the entire music file. Store it in memory somehow
			}
			if(type == LIST){
			    char* filename;
			    //TODO: call recv, and use that buffer to print out each string name.
			    //strings will be separated by /0	
		}
	

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

byte* create_message(message* msg, int msgType)
{
	filestate currState;
	msg->num_bytes = update_files(&currState);//TODO: THIS NEEDS TO RETURN NUM BYTES, NOT NUM FILES
	msg->filename_length = 0; //Not used by client
	msg->type = (message_type) msgType;
	return currState;

}
