#define OK 0
#define MAX 200

#include <stdio.h>
#include <stdlib.h>

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

    int powersSum = 0;

    while (!feof(fd)) {
        char lineArr[MAX];
        char *line = lineArr;
        
        if (fgets(lineArr, MAX, fd) != NULL) {

            while (!isDigit(*line)) line++;

            int necessaryRed = 0;
            int necessaryGreen = 0;
            int necessaryBlue = 0;

            while (!isDigit(*line)) line++;
            while (*line != '\0') {
            
                int colorAmount = strtol(line, &line, 10);
                while (*line == ' ') line++;
                
                switch (*line) {
                case 'r':
                    if (colorAmount > necessaryRed) necessaryRed = colorAmount;
                    break;
                case 'g':
                    if (colorAmount > necessaryGreen) necessaryGreen = colorAmount;
                    break;
                case 'b':
                    if (colorAmount > necessaryBlue) necessaryBlue = colorAmount;
                    break;      
                }

                while (!isDigit(*line) && *line != '\0') line++;
            }

            int power = necessaryRed * necessaryGreen * necessaryBlue;
            powersSum += power;
        }
    }

    fprintf(stdout, "Power: %d\n", powersSum);

    return OK;
}