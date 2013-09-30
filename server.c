#include <sys/stat.h>
#include "data_structs.h"
#include "file_util.h"

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