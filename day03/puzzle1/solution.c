#define OK 0
#define MAX 200

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isDigit(char c) {
    return c >= '0' && c <= '9';
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

    int partNumberSum = 0;

    int lineSize = strlen(lines[0]) - 1;

    for (int i = 0; i < size; i++) {
        char *current = lines[i];
        int desp = 0;
        while (*current != '\0') {
            if (isDigit(*current)) {
                char *endPointer;
                int number = strtol(current, &endPointer, 10);
                int isPartNumber = 0;
                while (isDigit(*current) && !isPartNumber) {
                    if (i == 0) {
                        if (desp == 0) {
                            isPartNumber = (lines[i][desp + 1] != '.'     && !isDigit(lines[i][desp + 1])) ||
                            (lines[i + 1][desp] != '.'     && !isDigit(lines[i + 1][desp])) ||
                            (lines[i + 1][desp + 1] != '.' && !isDigit(lines[i + 1][desp + 1]));
                        }
                        else if (desp == lineSize - 1) {
                            isPartNumber = (lines[i][desp - 1] != '.' && !isDigit(lines[i][desp - 1])) ||
                            (lines[i + 1][desp - 1] != '.' && !isDigit(lines[i + 1][desp - 1])) ||
                            (lines[i + 1][desp] != '.'     && !isDigit(lines[i + 1][desp]));
                        }
                        else {
                            isPartNumber = (lines[i][desp - 1] != '.' && !isDigit(lines[i][desp - 1])) ||
                            (lines[i][desp + 1] != '.'     && !isDigit(lines[i][desp + 1])) ||
                            (lines[i + 1][desp - 1] != '.' && !isDigit(lines[i + 1][desp - 1])) ||
                            (lines[i + 1][desp] != '.'     && !isDigit(lines[i + 1][desp])) ||
                            (lines[i + 1][desp + 1] != '.' && !isDigit(lines[i + 1][desp + 1]));          
                        }
                    }
                    else if (i == size - 1) {
                        if (desp == 0) {
                            isPartNumber = (lines[i - 1][desp] != '.'     && !isDigit(lines[i - 1][desp])) ||
                            (lines[i - 1][desp + 1] != '.' && !isDigit(lines[i - 1][desp + 1])) ||
                            (lines[i][desp + 1] != '.'     && !isDigit(lines[i][desp + 1]));
                        }
                        else if (desp == lineSize - 1) {
                            isPartNumber = (lines[i - 1][desp - 1] != '.' && !isDigit(lines[i - 1][desp - 1])) ||
                            (lines[i - 1][desp] != '.'     && !isDigit(lines[i - 1][desp])) ||
                            (lines[i][desp - 1] != '.'     && !isDigit(lines[i][desp - 1]));
                        }
                        else {
                            isPartNumber = (lines[i - 1][desp - 1] != '.' && !isDigit(lines[i - 1][desp - 1])) ||
                            (lines[i - 1][desp] != '.'     && !isDigit(lines[i - 1][desp])) ||
                            (lines[i - 1][desp + 1] != '.' && !isDigit(lines[i - 1][desp + 1])) ||
                            (lines[i][desp - 1] != '.'     && !isDigit(lines[i][desp - 1])) ||
                            (lines[i][desp + 1] != '.'     && !isDigit(lines[i][desp + 1]));
                        }
                    }
                    else {
                        if (desp == 0) {
                            isPartNumber = (lines[i - 1][desp] != '.'     && !isDigit(lines[i - 1][desp])) ||
                            (lines[i - 1][desp + 1] != '.' && !isDigit(lines[i - 1][desp + 1])) ||
                            (lines[i][desp + 1] != '.'     && !isDigit(lines[i][desp + 1])) ||
                            (lines[i + 1][desp] != '.'     && !isDigit(lines[i + 1][desp])) ||
                            (lines[i + 1][desp + 1] != '.' && !isDigit(lines[i + 1][desp + 1]));
                        }
                        else if (desp == lineSize - 1) {
                            isPartNumber = (lines[i - 1][desp - 1] != '.' && !isDigit(lines[i - 1][desp - 1])) ||
                            (lines[i - 1][desp] != '.'     && !isDigit(lines[i - 1][desp])) ||
                            (lines[i][desp - 1] != '.'     && !isDigit(lines[i][desp - 1])) ||
                            (lines[i + 1][desp - 1] != '.' && !isDigit(lines[i + 1][desp - 1])) ||
                            (lines[i + 1][desp] != '.'     && !isDigit(lines[i + 1][desp]));
                        }
                        else {
                            isPartNumber = (lines[i - 1][desp - 1] != '.' && !isDigit(lines[i - 1][desp - 1])) ||
                            (lines[i - 1][desp] != '.'     && !isDigit(lines[i - 1][desp])) ||
                            (lines[i - 1][desp + 1] != '.' && !isDigit(lines[i - 1][desp + 1])) ||
                            (lines[i][desp - 1] != '.'     && !isDigit(lines[i][desp - 1])) ||
                            (lines[i][desp + 1] != '.'     && !isDigit(lines[i][desp + 1])) ||
                            (lines[i + 1][desp - 1] != '.' && !isDigit(lines[i + 1][desp - 1])) ||
                            (lines[i + 1][desp] != '.'     && !isDigit(lines[i + 1][desp])) ||
                            (lines[i + 1][desp + 1] != '.' && !isDigit(lines[i + 1][desp + 1]));
                        }
                    }

                    if (isPartNumber) partNumberSum += number;
                    current++, desp++;
                }
                desp += endPointer - current;
                current = endPointer;
            }
            else current++, desp++;
        }
    }

    fprintf(stdout, "Part numbers sum: %d", partNumberSum);

    for (int i = 0; i < size; i++) free(lines[i]);
    free(lines);

    return OK;
}