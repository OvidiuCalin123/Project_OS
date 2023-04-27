#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include "regularFileOptions.c"

enum fileTypes{regularFile, directory, symbolicLink};

void showMenu(int fileType){

    if(fileType == regularFile){

        printf("\n--------MENU--------\n\n");
        printf("name (-n)\nsize (-d)\nhard link count (-h)\ntime of last modification (-m)\naccess rights (-a)\ncreate symlink (-l)\n");

    }else if(fileType == directory){

        printf("\n--------MENU--------\n\n");
        printf("name (-n)\nsize (-d)\nfiles with .c extension (-c)\naccess rights (-a)\n");

    }else if(fileType == symbolicLink){

        printf("\n--------MENU--------\n\n");
        printf("name (-n)\ndelete symlink (-l)\nsize of symlink (-d)\nsize of target file (-t)\naccess rights (-a)\n");

    }

}

int showFiles(int argc, char *argv[]){

    printf("\n\n");

    for(int i = 1; i < argc; i++){

        struct stat sb;

        printf("\n");

        if (stat(argv[i], &sb) == -1) {

            perror("Unknown Error");
            return -1;
        }

        if (S_ISREG(sb.st_mode)) {

            printf("File name: %s  File type: Regular file \n", argv[i]);
            showMenu(regularFile);

            regularFileHandle(argv[i]);

        } else if (S_ISDIR(sb.st_mode)) {

            printf("File name: %s  File type: Directory \n", argv[i]);
            showMenu(directory);

        } else if (S_ISLNK(sb.st_mode)) {

            printf("File name: %s  File type: Symbolic link \n", argv[i]);
            showMenu(symbolicLink);

        }

    }

    printf("\n\n\n");

}