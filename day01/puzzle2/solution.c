#define OK 0
#define MAX 100

#include <stdio.h>

char *numbers[9];

int getDigit(char *line) {
    if (*line >= '0' && *line <= '9') return (*line - 48);
    
    int digit = -1;
    for (int i = 0; i < 9 && digit == -1; i++) {
        char *number = numbers[i];

        int isDigit = 1;
        int desp = 0;
        for (int j = 0; isDigit && number[j] != '\0'; desp++, j++) {
            if ( *(line + desp) != number[j] ) isDigit = 0;
        }
        if (isDigit) digit = i + 1;
    }

    return digit;
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

    numbers[0] = "one\0";
    numbers[1] = "two\0";
    numbers[2] = "three\0";
    numbers[3] = "four\0";
    numbers[4] = "five\0";
    numbers[5] = "six\0";
    numbers[6] = "seven\0";
    numbers[7] = "eight\0";
    numbers[8] = "nine\0";

    int calibrationSum = 0;

    while (!feof(fd)) {
        char line[MAX];
        int first = -1;
        int last = -1;

        if (fgets(line, MAX, fd) != NULL) {
            int desp = 0;
            while (*(line + desp) != '\0') {
                int digit = getDigit(line + desp);
                
                if (digit != -1) {
                    if (first == -1) first = digit;
                    last = digit;
                }
                desp++;
            }
        }
        
        calibrationSum += 10*first + last;
    }

    fprintf(stdout, "Calibration: %d\n", calibrationSum);

    return OK;
}