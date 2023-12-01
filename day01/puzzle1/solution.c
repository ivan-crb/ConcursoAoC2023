#define OK 0

#include <stdio.h>

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

    int calibrationSum = 0;

    char c = fgetc(fd);

    while (c != EOF) {
        int first = -1;
        int last = -1;

        while (c != EOF && c != '\n') {
            if (isDigit(c)) {
                if (first == -1) first = (c - 48);
                last = (c - 48);
            }
            c = fgetc(fd);
        }
        
        calibrationSum += 10*first + last;
        c = fgetc(fd);
    }

    fprintf(stdout, "Calibration: %d", calibrationSum);

    return OK;
}
