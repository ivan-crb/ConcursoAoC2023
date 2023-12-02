#define OK 0
#define MAX 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int maxRedCubes = 12;
int maxGreenCubes = 13;
int maxBlueCubes = 14;

int isDigit(char c) {
    if (c >= '0' && c <= '9') return 1;
    return 0;
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

    int gamesSum = 0;

    while (!feof(fd)) {
        char lineArr[MAX];
        char *line = lineArr;
        
        if (fgets(lineArr, MAX, fd) != NULL) {
            int gameIsPossible = 1;

            while (!isDigit(*line)) line++;
            int gameNumber = strtol(line, &line, 10);

            while (!isDigit(*line)) line++;
            while (*line != '\0' && gameIsPossible) {
            
                int colorAmount = strtol(line, &line, 10);
                while (*line == ' ') line++;
                
                switch (*line) {
                case 'r':
                    if (colorAmount > maxRedCubes) gameIsPossible = 0;
                    break;
                case 'g':
                    if (colorAmount > maxGreenCubes) gameIsPossible = 0;
                    break;
                case 'b':
                    if (colorAmount > maxBlueCubes) gameIsPossible = 0;
                    break;      
                }

                while (!isDigit(*line) && *line != '\0') line++;
            }

            if (gameIsPossible) gamesSum += gameNumber;
        }
    }

    fprintf(stdout, "Sum: %d\n", gamesSum);

    return OK;
}