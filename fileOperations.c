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

    // Close the file
    close(fd);

}

void printNrOfFileLines(char fileName[]){
    
    printf("\nNumber of Lines:\n\n");

    int fp = open(fileName, O_RDONLY);
    if (fp == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    int numLines = 0;
    int numBytes;

    while ((numBytes = read(fp, buffer, 1024)) > 0) {
        for (int i = 0; i < numBytes; i++) {
            if (buffer[i] == '\n') {
                numLines++;
            }
        }
        
    }

    if (numBytes == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("%d\n\n", numLines);
    close(fp);
}

int createFileD(char dirName[]){

    char filename[256];

    snprintf(filename, sizeof(filename), "%s_file.txt", dirName);

    int fd = open(filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

    if (fd == -1) {
        printf("Error: cannot create file %s\n", filename);
        return 1;
    }

    const char* msg = "\n";

    size_t len = strlen(msg);
    ssize_t written = write(fd, msg, len);

    if (written == -1) {
        printf("Error: cannot write to file %s\n", filename);
        close(fd);
        return 1;
    }

    close(fd);
}