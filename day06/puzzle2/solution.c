#define OK 0
#define MAX 100

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int countDigits(int num) {
    if (num == 0) return 0;
    else return 1 + countDigits(num / 10);
}

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
   
    long long time = 0;
    long long recordDistance = 0;

    char *current = (char *) calloc(MAX, sizeof(char));
    
    while(!feof(fd)) {
        fgets(current, MAX, fd);
        char firstChar = *current;
        while (*current != ':') current++;
        while (*current != '\n' && *current != '\0') {
            if (isDigit(*current)) {
                long long number = strtol(current, &current, 10);
                if (firstChar == 'T') time = time*pow(10, countDigits(number)) + number;
                else if (firstChar == 'D') recordDistance = recordDistance*pow(10, countDigits(number))  + number;
            }
            else current++;
        }
    } 

    long ways = 0;
    for (long long j = 1; j < time; j++) {
        long long travelDistance = j * (time - j);
        if (travelDistance > recordDistance) ways++;
    }
    
    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tNumber of ways to beat the record: %lld\n", ways);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    free(current);

    return OK;
}