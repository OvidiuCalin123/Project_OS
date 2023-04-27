#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>

void regularFileHandle(char fileName[]){

    char option[4] = "";

    printf("\n\n");

    printf("Enter an option: ");

    scanf("%s", option);

    while(EOF){

        if (strcmp(option, "-n") == 0) {
            printf("%s \n", fileName);
        }
        break;

    }

}
