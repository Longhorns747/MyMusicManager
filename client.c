#include <stdio.h>
#include <string.h>
#include "data_structs.h"

const char selections[4][5] = {"LEAVE", "LIST", "PULL", "DIFF"};

int user_prompt();

int main()
{
	int cont = 1;

	printf("Welcome to your Music Manager :D!\n");

	while(cont)
	{
		cont = user_prompt();
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