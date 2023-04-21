#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>

#include "displayFiles.c"

int main(int argc, char *argv[]){

    if(argc > 1){

        showFiles(argc, argv);

    }

}