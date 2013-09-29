#ifndef FILE_UTIL_H
#define FILE_UTIL_H
 
#include <openssl/sha.h>
#include <dirent.h>
#include "data_structs.h"
#include <sys/stat.h>

typedef unsigned char byte;

byte* load_file(char fileName[], off_t fileSize)
{
	//Open an I/O stream to the file
	FILE* fileStream;
	fileStream = fopen(fileName, "r");
	int currChar = fgetc(fileStream);
	byte* fileBuf = malloc(fileSize);

	int i = 0;
	while(currChar != EOF)
	{
		fileBuf[i] = (byte)currChar;
		currChar = fgetc(fileStream);
		i++;
	}

	fclose(fileStream);
	return fileBuf;
}

byte* get_unique_id(char fileName[], off_t fileSize)
{ 

	byte* payload;
	payload = load_file(fileName, fileSize);

	byte* hash;
	hash = (char *) malloc(sizeof(short) * 20);
	SHA1(payload, sizeof(payload), hash);
	return hash;
}

#endif
