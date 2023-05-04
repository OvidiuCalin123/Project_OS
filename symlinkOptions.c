#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>

void symlinkHandle(char symlinkName[]){

    struct stat fileStat;

    char option[4] = "";

    printf("\n\n");
    printf("Enter an option: ");
    scanf("%s", option);

    if (strcmp(option, "-n") == 0) {
        printf("%s \n", symlinkName);

    } else if (strcmp(option, "-l") == 0) {
        
        // delete the symlink and return;

    } else if (strcmp(option, "-d") == 0) {
        if (stat(symlinkName, &fileStat) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);

        }
        
        printf("Size of %s is %ld bytes.\n", symlinkName, fileStat.st_size);

    } else if (strcmp(option, "-t") == 0) {

        struct stat targetStat;

        if (lstat(symlinkName, &fileStat) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (stat(symlinkName, &targetStat) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }
        
        printf("Size of the target file is %ld bytes.\n", targetStat.st_size);

    } else if(strcmp(option, "-a") == 0){

        if (stat(symlinkName, &fileStat) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        // User permissions
        printf("\nUser:\n");
        printf("Read - %s\n", (fileStat.st_mode & S_IRUSR) ? "yes" : "no");
        printf("Write - %s\n", (fileStat.st_mode & S_IWUSR) ? "yes" : "no");
        printf("Exec - %s\n", (fileStat.st_mode & S_IXUSR) ? "yes" : "no");

        // Group permissions
        printf("\nGroup:\n");
        printf("Read - %s\n", (fileStat.st_mode & S_IRGRP) ? "yes" : "no");
        printf("Write - %s\n", (fileStat.st_mode & S_IWGRP) ? "yes" : "no");
        printf("Exec - %s\n", (fileStat.st_mode & S_IXGRP) ? "yes" : "no");

        // Other permissions
        printf("\nOthers:\n");
        printf("Read - %s\n", (fileStat.st_mode & S_IROTH) ? "yes" : "no");
        printf("Write - %s\n", (fileStat.st_mode & S_IWOTH) ? "yes" : "no");
        printf("Exec - %s\n", (fileStat.st_mode & S_IXOTH) ? "yes" : "no");

    } else{

        printf("Invalid option!\n");
        printf("\n--------MENU--------\n\n");
        printf("name (-n)\ndelete symlink (-l)\nsize of symlink (-d)\nsize of target file (-t)\naccess rights (-a)\n");

        symlinkHandle(symlinkName);

    }


}
