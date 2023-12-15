#define OK 0
#define MAX 25000
#define ll long long

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Error: No input file provided\n");
        return OK;
    }

    FILE *fd = fopen(argv[1], "r");

    if (fd == NULL) {
        fprintf(stderr, "Error: Couldn't open input file\n");
        return OK;
    }

    clock_t startingTime = clock();


    char *line = calloc(MAX, sizeof(char *));
    fgets(line, MAX, fd);

    int hashSum = 0;

    while (*line != '\0') {
        int currentHash = 0;

        while (*line != ',' && *line != '\0' && *line != '\n') {
            currentHash += *line;
            currentHash *= 17;
            currentHash = currentHash % 256;
            line++;
        }
        line++;

        hashSum += currentHash;
    }


    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tTotal load: %d\n", hashSum);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    fclose(fd);

    return OK;
}