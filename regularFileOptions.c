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
#include <sys/wait.h>
#include "processesHelper.c"

int pid_execute_option;
int pid_regular_file_extension;

void checkRegularFileExtension(char fileName[]){

    if((pid_regular_file_extension=fork()) == 0){

        char *extension = strrchr(fileName, '.');

        if (extension != NULL && strcmp(extension, ".c") == 0) {

            printf("%s is a C file\n", fileName);

        } else {

            printf("\nFile Content:\n\n");
            FILE *fp = fopen(fileName, "r");

            if (fp == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            
            char buffer[1024];

            while (fgets(buffer, 1024, fp) != NULL) {
                printf("%s", buffer);
            }
            
            printf("\n\n");
            fclose(fp);
        }

        exit(pid_regular_file_extension);
    
    }else if(pid_regular_file_extension < 0){

        perror("fork error regular file print lines");
        exit(EXIT_FAILURE);
    }

}

void regularFileHandle(char fileName[]){

    struct stat fileStat;

    char option[4] = "";

    printf("\n\n");
    printf("Enter an option: ");
    scanf("%s", option);

    checkRegularFileExtension(fileName);

    if((pid_execute_option=fork()) == 0){

        if (strcmp(option, "-n") == 0) {
            
            printf("%s \n", fileName);

        } else if (strcmp(option, "-d") == 0) {
            if (stat(fileName, &fileStat) == -1) {
                perror("stat");
                exit(EXIT_FAILURE);

            }
            
            printf("Size of %s is %ld bytes.\n", fileName, fileStat.st_size);

        } else if(strcmp(option, "-m") == 0){

            if (stat(fileName, &fileStat) == -1) {
                perror("stat");
                exit(EXIT_FAILURE);
            }

            char *timeStr = ctime(&fileStat.st_mtime);
            printf("Time of last modification is: %s", timeStr);
            
        } else if(strcmp(option, "-a") == 0){
            if (stat(fileName, &fileStat) == -1) {
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

        } else if (strcmp(option, "-l") == 0) {

            char linkName[100];

            printf("Enter the name of the symbolic link: ");
            scanf("%s", linkName);

            if (symlink(fileName, linkName) == -1) {
                perror("symlink");
                exit(EXIT_FAILURE);
            }

        } else{

            printf("Invalid option!\n");
            printf("\n--------MENU--------\n\n");
            printf("name (-n)\nsize (-d)\nhard link count (-h)\ntime of last modification (-m)\naccess rights (-a)\ncreate symlink (-l)\n");

            regularFileHandle(fileName);

        }

        exit(pid_execute_option);

    } else if(pid_execute_option < 0){ 

        perror("fork error");
        exit(EXIT_FAILURE);
    }

    if(pid_execute_option > 0 && pid_regular_file_extension > 0){
        waitForProcessToFinish(pid_execute_option);
        waitForProcessToFinish(pid_regular_file_extension);
    }


}
