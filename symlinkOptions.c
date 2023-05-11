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

int pid_execute_options_symlink;
int pid_change_permissions;

void changePermissions(char symbolicLinkName[]){

    if((pid_change_permissions=fork()) == 0){

        if(open(symbolicLinkName, O_RDONLY) != -1){

            char changePermsUser[50] = "";
            char changePermsGroup[50] = "";
            char changePermsOthers[50] = "";

            sprintf(changePermsUser, "chmod u+rwx,o= %s", symbolicLinkName);
            sprintf(changePermsGroup, "chmod g+rw,g-x %s", symbolicLinkName);
            sprintf(changePermsOthers, "chmod o+t %s", symbolicLinkName);
            
            system(changePermsUser);
            system(changePermsGroup);
            system(changePermsOthers);

        }

        exit(pid_change_permissions);
    }

}

int checkValidOptionSym(char option[]){

    char *options[] = {"-n", "-l", "-d", "-t", "-a"};

    int valid_option = 0;

    for (int i = 0; i < 5; i++) {
        if (strcmp(option, options[i]) == 0) {
            return 1;
        }
    }

    return -1;
}


void symlinkHandle(char symlinkName[]){

    struct stat fileStat;

    char option[4] = "";

    printf("\n\n");
    printf("Enter an option: ");
    scanf("%s", option);

    if(checkValidOption(option) > 0){

        changePermissions(symlinkName);

        //chmod +rwx
        //chmod g+rwx
        //chmod o+no permissions

        if((pid_execute_options_symlink=fork()) == 0){

            if (strcmp(option, "-n") == 0) {
                printf("%s \n", symlinkName);

            } if (strcmp(option, "-l") == 0) {

                int status = unlink(symlinkName);

                if (status == 0) {
                    printf("Symlink deleted successfully.\n");
                } else {
                    perror("Could not delete link");
                    exit(1);
                }

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

            }

            exit(pid_execute_options_symlink);

        }

        sleep(1);
         
         waitForProcessToFinish(pid_execute_options_symlink);

        sleep(1);

         waitForProcessToFinish(pid_change_permissions);

    }else{

        printf("Invalid option!\n");
        printf("\n--------MENU--------\n\n");
        printf("name (-n)\ndelete symlink (-l)\nsize of symlink (-d)\nsize of target file (-t)\naccess rights (-a)\n");

        symlinkHandle(symlinkName);

    }


}
