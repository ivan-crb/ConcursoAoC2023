#define OK 0
#define MAX 300

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
        if (fgets(line, MAX, fd) != NULL) lines[size++] = line;
    }

    int totalPoints = 0;

    for (int i = 0; i < size; i++) {
        char *current = lines[i];

        while (*current != ':') current++;

        int winningNumbers[MAX];
        int winningAmount = 0;
        while (*current != '|') {
            if (isDigit(*current)) {
                int number = strtol(current, &current, 10);
                winningNumbers[winningAmount++] = number;
            }
            else current++;
        }

        int matches = 0;
        while (*current != '\0') {
            if (isDigit(*current)) {
                int number = strtol(current, &current, 10);
                int contains = 0;
                for (int j = 0; j < winningAmount && !contains; j++) {
                    if (number == winningNumbers[j]) contains = 1;
                }
                if (contains) matches++;
            }
            else current++;
        }

        if (matches > 0) totalPoints += pow(2, matches - 1);

    }

    fprintf(stdout, "Total points: %d", totalPoints);

    for (int i = 0; i < size; i++) free(lines[i]);
    free(lines);

    return OK;
}