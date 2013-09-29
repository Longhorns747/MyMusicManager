#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "data_structs.h"
#include "file_util.h"

int main()
{
	return 0;
}

//Needed for the scandir function used below
static int one()
{
	return 1;
}

int update_files(filestate* state)
{
    struct dirent **files;

    int numFiles = scandir("./", &files, one, qsort);
    music_file *fileList;
    fileList=(music_file*) malloc(numFiles * sizeof(music_file));

    if(numFiles >= 0)
    {
    	for(int i = 0; i < numFiles; i++)
    	{
    		struct stat fileAttributes;
    		stat(files[i]->d_name, &fileAttributes);
    		fileList[i].filename = files[i]->d_name;
    		fileList[i].ID = get_unique_id(files[i]->d_name, fileAttributes.st_size);
    	}
    }
    else
    {
    	return 0;
    }

    state->music_files = fileList;
    return numFiles;
}


