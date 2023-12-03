#define OK 0
#define MAX 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int getPartNumbers(char* line, int desp, int* partNumbers) {
    int size = 0;
    char* current = line;

    while (*current != '\0') {
        char* endPtr;
        
        int number = strtol(current, &endPtr, 10);
        if (number) {
            int start = current - line;
            int end = endPtr - line - 1;

            if ( (start >= (desp - 1) && start <= (desp + 1 )) || (end >= (desp - 1) && end <= (desp + 1)) ) partNumbers[size++] = number;
            current = endPtr;
        }
        else current++;

    }
    return size;
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

    char **lines = calloc(MAX, sizeof(char *));
    int size = 0;

    while (!feof(fd)) {
        char *line = calloc(MAX, sizeof(char));
        if (fgets(line, MAX, fd) != NULL) *(lines + size++) = line;
    }

    int gearRatioSum = 0;

    int lineSize = strlen(lines[0]) - 1;

    for (int i = 0; i < size; i++) {
        char *current = lines[i];
        int desp = 0;
        while (*current != '\0') {
            if (*current == '*') {

                int *partNumbersMid = calloc(MAX, sizeof(int));
                int partNumbersAmountMid = getPartNumbers(lines[i], desp, partNumbersMid);
                
                int *partNumbersTop = calloc(MAX, sizeof(int));
                int partNumbersAmountTop;
                if (i > 0) partNumbersAmountTop = getPartNumbers(lines[i - 1], desp, partNumbersTop);

                int *partNumbersBot = calloc(MAX, sizeof(int));
                int partNumbersAmountBot;
                if (i < lineSize - 1 ) partNumbersAmountBot = getPartNumbers(lines[i + 1], desp, partNumbersBot);

                if ((partNumbersAmountMid + partNumbersAmountTop + partNumbersAmountBot) >= 2) {
                    int gearRatio = 1;

                    for (int j = 0; j < partNumbersAmountMid; j++) gearRatio *= partNumbersMid[j];
                    for (int j = 0; j < partNumbersAmountTop; j++) gearRatio *= partNumbersTop[j];
                    for (int j = 0; j < partNumbersAmountBot; j++) gearRatio *= partNumbersBot[j];

                    gearRatioSum += gearRatio;
                }
            }
            current++, desp++;
        }
    }

    fprintf(stdout, "Gear ratio sum: %d", gearRatioSum);

    for (int i = 0; i < size; i++) free(lines[i]);
    free(lines);

    return OK;
}