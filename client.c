#include <string.h>
#include "data_structs.h"
#include "file_util.h"

const char selections[4][5] = {"LEAVE", "LIST", "PULL", "DIFF"};

int user_prompt();
void create_message(message* msg, int msgType);

int main()
{
	int userChoice = 1;

	printf("Welcome to your Music Manager :D!\n");

	while(userChoice)
	{
		userChoice = user_prompt();
		message* msg;
		create_message(msg, userChoice);
		printf("Created message with type: %d file 3: %s\n", msg->type, msg->state->music_files[2].filename);
		free_files(msg->state);
	}
	
	return 0;
}

int user_prompt()
{
	char selection[5];
	printf("Please make a selection from one of the following: LEAVE, LIST, PULL, DIFF\n");
	scanf("%s", selection);

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
