#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "data_structs.h"
#include "file_util.h"

int update_files(filestate* state);
void free_files(filestate* state);
int alphasort(const struct dirent ** a, const struct dirent **b);

int main()
{
    filestate* currState;
    int numFiles = update_files(currState);

    printf("You were... %d successful!\n", numFiles);

    for(int i = 0; i < numFiles; i++){
        music_file currFile = currState->music_files[i];
        printf("File%d: %s\n", i, currFile.filename);
    }

    free_files(currState);
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
