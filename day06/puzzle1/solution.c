#define OK 0
#define MAX 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int isLetter(char c) {
    return c >= 'a' && c <= 'z';
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
   
    int millis[MAX];
    int millisLength = 0;
    int distances[MAX];
    int distancesLength = 0;

    char *current = (char *) calloc(MAX, sizeof(char));
    
    while(!feof(fd)) {
        fgets(current, MAX, fd);
        char firstChar = *current;
        while (*current != ':') current++;
        while (*current != '\n' && *current != '\0') {
            if (isDigit(*current)) {
                int number = strtol(current, &current, 10);
                if (firstChar == 'T') millis[millisLength++] = number;
                else if (firstChar == 'D') distances[distancesLength++] = number;
            }
            else current++;
        }
    } 

    int totalWays = 0;
    
    for (int i = 0; i < millisLength; i++) {
        int time = millis[i];
        int recordDistance = distances[i];

        int ways = 0;
        for (int j = 1; j < time; j++) {
            int travelDistance = j * (time - j);
            if (travelDistance > recordDistance) ways++;
        }

        if (totalWays == 0) totalWays += ways;
        else totalWays *= ways;
    }
    
    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tNumber of ways to beat the record: %d\n", totalWays);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    free(current);

    return OK;
}