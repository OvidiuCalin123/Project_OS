#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define BUFFER_SIZE 100

int count = 0;

void statistica(char buffer[], int flag) {
    for (int i = 0; i < flag; i++) {
        if (isalnum(buffer[i])) {
            count += 1;
        }
    }
}

void sciere(int fout) {
    struct stat fileStat;
    if (fstat(fout, &fileStat) == -1) {
        perror("Could not get file status");
        exit(EXIT_FAILURE);
    }

    char bufferOut[BUFFER_SIZE];
    sprintf(bufferOut, "malfanum: %d, id_owner: %u", count, fileStat.st_uid);

    if (write(fout, bufferOut, strlen(bufferOut)) < 0) {
        perror("Could not write to file");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("Not enough arguments!");
        exit(EXIT_FAILURE);
    }

    int fileIn = open(argv[1], O_RDONLY);
    if (fileIn == -1) {
        perror("Could not open input file");
        exit(EXIT_FAILURE);
    }

    int fileOut = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fileOut == -1) {
        perror("Could not create output file");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(fileIn, buffer, BUFFER_SIZE)) > 0) {
        statistica(buffer, bytesRead);
        write(fileOut, buffer, bytesRead);
    }

    if (bytesRead == -1) {
        perror("Could not read input file");
        exit(EXIT_FAILURE);
    }

    sciere(fileOut);

    if (close(fileIn) == -1) {
        perror("Could not close input file");
        exit(EXIT_FAILURE);
    }

    if (close(fileOut) == -1) {
        perror("Could not close output file");
        exit(EXIT_FAILURE);
    }

    return 0;
}
