#ifndef FILE_UTIL_H
#define FILE_UTIL_H
 
#include <openssl/sha.h>
#include <dirent.h>
#include "data_structs.h"
#include <sys/stat.h>

typedef unsigned char byte;

int alphasort(const struct dirent ** a, const struct dirent **b);

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

//Needed for the scandir function used below
static int one()
{
	return 1;
}

int update_files(filestate* state)
{
    struct dirent **files;

    int numFiles = scandir("./", &files, one, alphasort);

    printf("Scanned directory\n");

    music_file *fileList;
    fileList=(music_file*) malloc(numFiles * sizeof(music_file));
    printf("Allocated space for %d files\n", numFiles);

    if(numFiles >= 0)
    {
    	for(int i = 0; i < numFiles; i++)
    	{
    		struct stat fileAttributes;
    		stat(files[i]->d_name, &fileAttributes);
    		fileList[i].filename = files[i]->d_name;
    		fileList[i].ID = get_unique_id(files[i]->d_name, fileAttributes.st_size);
            printf("Made musicfile for %s\n", fileList[i].filename);
    	}
    }
    else
    {
    	return 0;
    }

    state->music_files = fileList;
    state->numFiles = numFiles;
    return numFiles;
}

void free_files(filestate* state)
{
    free(state->music_files);
}

//Returns the filestate of files different in state2 from state1 (state2 - state1)
//Returns NULL if same state
filestate* delta(filestate* state1, filestate* state2)
{
    int length;

    //Iterate to length of smaller list
    if(state1->numFiles < state2->numFiles)
        length = state1->numFiles;
    else
        length = state2->numFiles;

    int diffCount = 0;

    for(int i = 0; i < length; i++){
        if(!strcmp(state2->music_files[i].ID, state1->music_files[i].ID))
            diffCount++;
    }

    //Need to take into account more files in state2 than state1
    if(state2->numFiles > state1-> numFiles)
        diffCount += state2->numFiles - state1->numFiles;

    //Return null if same state
    if(diffCount == 0)
        return NULL;

    filestate *res;
    res->numFiles = diffCount;

    music_file *fileList = malloc(sizeof(music_file) * diffCount);
    int j = 0;

    //Copy difference files over
    for(int i = 0; i < length; i++){
        if(!strcmp(state2->music_files[i].ID, state1->music_files[i].ID))
        {
            fileList[j] = state2->music_files[i];
            j++;
        }
    }

    //If more files in state2 than state1, need to copy extras over
    if(j != diffCount)
    {
        for(int i = state1->numFiles; i < state2->numFiles; i++){
            fileList[i] = state2->music_files[i];
        }
    }

    res->music_files = fileList;
    return res;
}

#endif
