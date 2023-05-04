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

void directoryHandle(char directoryName[]){

    struct stat fileStat;

    char option[4] = "";

    printf("\n\n");
    printf("Enter an option: ");
    scanf("%s", option);

    if (strcmp(option, "-n") == 0) {
        printf("%s \n", directoryName);

    } else if (strcmp(option, "-d") == 0) {
        if (stat(directoryName, &fileStat) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);

        }
        
        printf("Size of %s is %ld bytes.\n", directoryName, fileStat.st_size);

    } else if (strcmp(option, "-c") == 0) {
        
        DIR *dir;
        struct dirent *entry;
        struct stat fileStat;
        int count = 0;

        dir = opendir(directoryName);
        if (dir == NULL) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }

        while ((entry = readdir(dir)) != NULL) {
            char filePath[500];
            snprintf(filePath, 500, "%s/%s", directoryName, entry->d_name);

            if (stat(filePath, &fileStat) == -1) {
                perror("stat");
                continue;
            }

            if (S_ISREG(fileStat.st_mode) && strstr(entry->d_name, ".c") != NULL) {
                count++;
            }
        }

        closedir(dir);
        printf("There are %d files with the .c extension in the directory.", count);

    } else if(strcmp(option, "-a") == 0){
        if (stat(directoryName, &fileStat) == -1) {
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
        printf("name (-n)\nsize (-d)\nfiles with .c extension (-c)\naccess rights (-a)\n");

        directoryHandle(directoryName);

    }


}
