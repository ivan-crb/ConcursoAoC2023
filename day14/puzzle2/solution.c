#define OK 0
#define MAX 12000
#define CYCLES 1000000000
#define ll long long

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int isContained (char ***arr, int arrLength, char **matrix, int rows, int cols) {
    int contained = -1;
    for (int i = 0; i < arrLength && contained < 0; i++) {
        int equal = 1;
        for (int j = 0; j < rows && equal; j++) {
            for (int k = 0; k < cols && equal; k++) {
                if (arr[i][j][k] != matrix[j][k]) equal = 0; 
            }
        }
        if (equal) contained = i;
    }
    return contained;
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

    char **matrix = calloc(MAX, sizeof(char *));
    int rowsAmount = 0;

    while(!feof(fd)) {
        char *row = calloc(MAX, sizeof(char));
        fgets(row, MAX, fd);

        matrix[rowsAmount++] = row; 
    } 
   
    int rowLength = 0;
    while (matrix[0][rowLength] != '\n' && matrix[0][rowLength] != '\0') rowLength++;

    char ***cycles = calloc(CYCLES, sizeof(char **));
    int cyclesAmount = 0;
    int loopStart = -1;
    int loopEnd = -1;
    for (ll n = 0; (n < (ll) 4 * CYCLES && loopStart < 0); n++) {
        switch (n % 4) {
            case 0: // Roll north
                for (int i = 0; i < rowsAmount; i++) {
                    for (int j = 0; j < rowLength; j++) {
                        if (matrix[i][j] == 'O') {
                            for (int k = i; k > 0 && matrix[k-1][j] == '.'; k--) {
                                matrix[k-1][j] = 'O';
                                matrix[k][j] = '.';
                            }
                        }
                    }
                }
                break;
            case 1: // Roll West
                for (int i = 0; i < rowsAmount; i++) {
                    for (int j = 0; j < rowLength; j++) {
                        if (matrix[i][j] == 'O') {
                            for (int k = j; k > 0 && matrix[i][k-1] == '.'; k--) {
                                matrix[i][k-1] = 'O';
                                matrix[i][k] = '.';
                            }
                        }
                    }
                }
            break;
            case 2: // Roll south
                for (int i = rowsAmount - 1; i >= 0; i--) {
                    for (int j = rowLength -1 ; j >= 0; j--) {
                        if (matrix[i][j] == 'O') {
                            for (int k = i; k < rowsAmount - 1 && matrix[k+1][j] == '.'; k++) {
                                matrix[k+1][j] = 'O';
                                matrix[k][j] = '.';
                            }
                        }
                    }
                }
            break;
            case 3: // Roll east
                for (int i = rowsAmount - 1; i >= 0; i--) {
                    for (int j = rowLength - 1 ; j >= 0; j--) {
                        if (matrix[i][j] == 'O') {
                            for (int k = j; k < rowLength - 1 && matrix[i][k+1] == '.'; k++) {
                                matrix[i][k+1] = 'O';
                                matrix[i][k] = '.';
                            }
                        }
                    }
                }
            break;
        }
        if (n % 4 == 3) {
            int pos = isContained(cycles, cyclesAmount, matrix, rowsAmount, rowLength);
            if (pos >= 0) {
                loopStart = pos;
                loopEnd = cyclesAmount;
            }

            cycles[cyclesAmount] = calloc(MAX, sizeof(char *));
            for (int i = 0; i < rowsAmount; i++) {
                char *row = calloc(MAX, sizeof(char));
                for (int j = 0; j < rowLength; j++) {
                    row[j] = matrix[i][j];
                }
                cycles[cyclesAmount][i] = row;
            }
            cyclesAmount++;
        }
    }

    int totalLoad = 0;

    char **lastMatrix = cycles[ ((CYCLES - 1 - loopStart) % (loopEnd - loopStart)) + loopStart ];
    for (int i = 0; i < rowsAmount; i++) {
        for (int j = 0; j < rowLength; j++) {
            if (lastMatrix[i][j] == 'O') {
                totalLoad += (rowsAmount - i);
            }
        }
    }


    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tTotal load: %d\n", totalLoad);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    fclose(fd);

    return OK;
}