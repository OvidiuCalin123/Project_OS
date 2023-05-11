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
#include "fileOperations.c"

int pid_execute_options_regular;
int pid_regular_file_extension;
int score = -1;

void checkRegularFileExtension(char fileName[]) {

    if ((pid_regular_file_extension = fork()) == 0) {

        int status;

        char *extension = strrchr(fileName, '.');

        if (extension != NULL && strcmp(extension, ".c") == 0) {

            score = computeScore(fileName);

            char buffer[1024];
            snprintf(buffer, sizeof(buffer), "%s: %d\n", fileName, score);

            int fd = open("grades.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            if (write(fd, buffer, strlen(buffer)) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }

            close(fd);

        } else {

            printNrOfFileLines(fileName);
        }

        exit(pid_regular_file_extension);

    } else if (pid_regular_file_extension < 0) {

        perror("fork error regular file print lines");
        exit(EXIT_FAILURE);
    }
}

int checkValidOption(char option[]){

    char *options[] = {"-n", "-d", "-h", "-l", "-m", "-a"};

    int valid_option = 0;

    for (int i = 0; i < 6; i++) {
        if (strcmp(option, options[i]) == 0) {
            return 1;
        }
    }

    return -1;
}


void regularFileHandle(char fileName[]){

    struct stat fileStat;

    char option[4] = "";

    printf("\n\n");
    printf("Enter an option: ");
    scanf("%s", option);

    if(checkValidOption(option) > 0){

        checkRegularFileExtension(fileName);

        if((pid_execute_options_regular=fork()) == 0){

            if (strcmp(option, "-n") == 0) {
            
                printf("%s \n\n", fileName);

            } else if (strcmp(option, "-d") == 0) {
                if (stat(fileName, &fileStat) == -1) {
                    perror("stat");
                    exit(EXIT_FAILURE);

                }
            
                printf("\nSize of %s is %ld bytes.\n\n", fileName, fileStat.st_size);

            } else if(strcmp(option, "-h") == 0){

                if (stat(fileName, &fileStat) == -1) {
                    perror("stat");
                    exit(EXIT_FAILURE);
                }

                printf("\nNumber of hard links: %lu\n\n", fileStat.st_nlink);
            
            }
            else if(strcmp(option, "-m") == 0){

                if (stat(fileName, &fileStat) == -1) {
                    perror("stat");
                    exit(EXIT_FAILURE);
                }

                char *timeStr = ctime(&fileStat.st_mtime);
                printf("\nTime of last modification is: %s\n", timeStr);
            
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
                printf("Exec - %s\n\n", (fileStat.st_mode & S_IXOTH) ? "yes" : "no");

            } else if (strcmp(option, "-l") == 0) {

                char linkName[100];

                printf("\nEnter the name of the symbolic link: ");
                scanf("%s", linkName);

                if (symlink(fileName, linkName) == -1) {
                    perror("symlink");
                    exit(EXIT_FAILURE);
                }

                printf("\n");

            }

            exit(pid_execute_options_regular);

        } else if(pid_execute_options_regular < 0){ 

            perror("fork error");
            exit(EXIT_FAILURE);
        }

        sleep(1);
        
        int status;

        waitForProcessToFinish(pid_execute_options_regular);

        sleep(1);

        waitForProcessToFinish(pid_regular_file_extension);

        sleep(1);

        if(score != -1){
            writeScoreToFile(fileName, score);
        }

    }else{

        printf("Invalid option!\n");
        printf("\n--------MENU--------\n\n");
        printf("name (-n)\nsize (-d)\nhard link count (-h)\ntime of last modification (-m)\naccess rights (-a)\ncreate symlink (-l)\n");

        regularFileHandle(fileName);

    }

    
      
}
