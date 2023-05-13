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
    
        char buffer[50];
        sprintf(buffer, "bash script.sh %s", fileName);
        system(buffer);
        
        int first_val, second_val;

        FILE *fp1 = fopen("log.txt", "r");
        if (fp1 == NULL) {
            printf("Error opening file\n");
        }

        fscanf(fp1, "%d %d", &first_val, &second_val);

        int score = 0;

        if (first_val == 0 && second_val == 0) {
            score = 10;
        } else if (first_val > 0) {
            score = 1;
        } else if (second_val > 10) { //pri
            score = 2;
        } else if(second_val <= 10){
            score = 2 + 8 * (10 - second_val) / 10;
        }

        fclose(fp1);


    return score;
}