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

void waitForProcessToFinish(int processId){

    int status;
    pid_t pid = waitpid(processId, &status, 0);

    if (pid == -1) {
        
        if (errno == ECHILD) {
            printf("Error: no child processes\n");
        } else if (errno == EINTR) {
            printf("Interrupted by a signal\n");
        } else {
            printf("Unknown error occurred\n");
        }
    } else {
        if (WIFEXITED(status)) {

            int exit_code = WEXITSTATUS(status);
            printf("The process with PID %d has ended with the exit code %d\n", pid, exit_code);

        } else {
            printf("\nThe process with PID %d has terminated abnormally\n", pid);
        }
    }

}

int computeScore(char fileName[]){
    
    char command[256];

    snprintf(command, 256, "gcc -Wall -Wextra -pedantic -std=c99 -fsyntax-only %s 2>&1 | grep -E -i '(error|warning):' > errors_warnings.txt", fileName);
    system(command);

    FILE *fp = fopen("errors_warnings.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int num_errors = 0;
    int num_warnings = 0;

    char line[256];

    while (fgets(line, 256, fp) != NULL) {
        if (strstr(line, "error") != NULL) {
            num_errors++;
        } else if (strstr(line, "warning") != NULL) {
            num_warnings++;
        }
    }

    fclose(fp);
    remove("errors_warnings.txt");

    int score = 0;

    if (num_errors == 0 && num_warnings <= 10) {
        score = 10 + 8 * (10 - num_warnings) / 10;
    } else if (num_errors == 0 && num_warnings > 10) {
        score = 2;
    } else {
        score = 1;
    }

    // printf("\nNumber of errors: %d\n", num_errors);
    // printf("Number of warnings: %d\n", num_warnings);
    // printf("Score: %d\n", score);

    return score;
}