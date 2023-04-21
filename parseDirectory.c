#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>

#define IS_REG(mode) ((mode) & S_IFREG)

int main(int argc, char *argv[]) {

    FILE *fisout;
    FILE *out;

    struct stat inf;
    struct stat inff;
    struct dirent *entry;

    if (argc != 3) {
        perror("Not enough arguments!");
        exit(EXIT_FAILURE);
    }

    char path[200];
    char buffer[200];
    DIR *dir;

    if((dir = opendir(argv[1])) != 0){

        if((fisout = fopen(argv[2], "w")) != 0){

            if(fstat(fileno(fisout), &inf) == 0){

                if(IS_REG(inf.st_mode)){

                    while((entry = readdir(dir)) != 0){

                        sprintf(path, "%s/%s", argv[1], entry->d_name);

                        if(stat(path, &inff) == 0 && IS_REG(inff.st_mode)){
                            if(access("path", R_OK) == 0){
                                sprintf(buffer, "n:%s no:%d\n access to read: %s", path, inff.st_blksize, "read");
                            }else{
                                sprintf(buffer, "n:%s no:%d\n access to read: %s", path, inff.st_blksize, "cant read");
                            }
                            
                            if(fputs(buffer, fisout) == EOF){
                                perror("Write error");
                            }
                        }
                    }

                }else{
                    perror("Output file is not a regular file");
                }

            }else{
                perror("fstat error");
            }

            fclose(fisout);

        }else{
            perror("Output file could not be opened");
        }

        closedir(dir);

    }else{
        perror("Input directory could not be opened");
    }

    return 0;
}
