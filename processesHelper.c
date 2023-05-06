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
            printf("The process with PID %d has terminated abnormally\n", pid);
        }
    }

}