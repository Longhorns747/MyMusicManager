#include <stdio.h>
#include <string.h>
#include "data_structs.h"

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
	printf("Please make a selection from one of the following: LIST, DIFF, PULL, LEAVE\n");
	scanf("%s", selection);

	char leave[] = "LEAVE";
	return strcmp(selection, leave);
}