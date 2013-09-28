#ifndef FILE_UTIL_H
#define FILE_UTIL_H
 
#include <openssl/sha.h>
#include <dirent.h>
#include "data_structs.h"
#include <sys/stat.h>

//short SHA_DIGEST_LENGTH = 20;

unsigned char* load_file(char fileName[], off_t fileSize)
{
	//Open an I/O stream to the file
	FILE* fileStream;
	fileStream = fopen(fileName, "r");
	int currChar = fgetc(fileStream);
	unsigned char* fileBuf = malloc(fileSize);

	int i = 0;
	while(currChar != EOF)
	{
		fileBuf[i] = (unsigned char)currChar;
		currChar = fgetc(fileStream);
		i++;
	}

	fclose(fileStream);
	return fileBuf;
}

unsigned char* get_unique_id(char fileName[], off_t fileSize)
{ 

	unsigned char* payload;
	payload = load_file(fileName, fileSize);

	unsigned char hash[20];
	SHA1(payload, sizeof(payload), hash);
	return hash;
}

#endif
