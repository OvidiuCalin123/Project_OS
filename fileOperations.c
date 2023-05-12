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

void writeScoreToFile(char fileName[], int score){

    int fd;
    char buf[100];

    // Open the file for writing
    fd = open("grades.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);

    if (fd < 0) {
        printf("Failed to open the file.\n");
        exit(1);
    }

    // Write the score to the file
    snprintf(buf, sizeof(buf), "%s: %d\n", fileName, score);
    write(fd, buf, strlen(buf));

    close(fd);

}

void printNrOfFileLines(char filename[]) {

    FILE *fp;
    char buffer[100];
    int count = 0;

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: Failed to open file.\n");
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        count++;
    }

    fclose(fp);

    printf("\nNumber of lines: %d\n\n", count);

}

int createFileD(char dirName[]) {

    int len = strlen(dirName);
    if (dirName[len-1] == '/') {
        dirName[len-1] = '\0';
    }

    char filename[256];

    sprintf(filename, "%s_file.txt", dirName);

    int fd = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        printf("Error: cannot create file %s\n", filename);
        return 1;
    }

    const char* msg = "\n";

    size_t msg_len = strlen(msg);
    ssize_t written = write(fd, msg, msg_len);

    if (written == -1) {
        printf("Error: cannot write to file %s\n", filename);
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
