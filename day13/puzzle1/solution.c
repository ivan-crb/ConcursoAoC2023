#define OK 0
#define MAX 12000

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct matrix_t {
    char **matrix;
    int rows;
    int columns;
} matrix;

int rowEquals(char *row1, char *row2, int rowLength) {
    int equal = 1;
    for (int i = 0; i < rowLength && equal; i++) {
        if (row1[i] != row2[i]) equal = 0;
    }
    return equal;
}

int findReflectionHorz (matrix matrix) {
    int firstRow = -1;
    int secondRow = -1;
    for (int i = 0; i < matrix.rows - 1 && firstRow < 0; i++) {
        char *row1 = matrix.matrix[i];
        char *row2 = matrix.matrix[i+1];

        int isReflection = 1;
        int up = i;
        int down = i+1;
        while (up >= 0 && down < matrix.rows && isReflection) {
            if (rowEquals(matrix.matrix[up], matrix.matrix[down], matrix.columns)) {
                up--;
                down++;
            }
            else isReflection = 0;
        }

        if (isReflection) {
            firstRow = i;
            secondRow = i+1;
        }
    }
    return secondRow;
}

int colEquals(char **matrix, int col1, int col2, int colLength) {
    int equal = 1;
    for (int i = 0; i < colLength && equal; i++) {
        if (matrix[i][col1] != matrix[i][col2]) equal = 0;
    }
    return equal;
}

int findReflectionVert (matrix matrix) {
    int firstCol = -1;
    int secondCol = -1;
    for (int i = 0; i < matrix.columns - 1 && firstCol < 0; i++) {
        char *row1 = matrix.matrix[i];
        char *row2 = matrix.matrix[i+1];

        int isReflection = 1;
        int left = i;
        int right = i+1;
        while (left >= 0 && right < matrix.columns && isReflection) {
            if (colEquals(matrix.matrix, left, right, matrix.rows)) {
                left--;
                right++;
            }
            else isReflection = 0;
        }

        if (isReflection) {
            firstCol = i;
            secondCol = i+1;
        }
    }
    return secondCol;
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

    matrix *patterns = calloc(MAX, sizeof(matrix));
    int patternsAmount = 0;
    
    int matrixStarted = 0;
    while(!feof(fd)) {
        char *row = calloc(MAX, sizeof(char));
        fgets(row, MAX, fd);
        if (!matrixStarted) {
            patterns[patternsAmount].matrix = calloc(MAX, sizeof(char *));
            matrixStarted = 1;
        }
        if (row[0] == '\n') {
            patternsAmount++;
            matrixStarted = 0;
        }
        else {
            patterns[patternsAmount].matrix[patterns[patternsAmount].rows++] = row;
        }
    }
    patternsAmount++;
    
    int rowsNum = 0;
    int columnsNum = 0;

    for (int i = 0; i < patternsAmount; i++) {

        char **pattern = patterns[i].matrix;
        
        int columns = 0;
        while (pattern[0][columns] != '\n') columns++;
        patterns[i].columns = columns;
    
        int row = findReflectionHorz(patterns[i]);

        int col = findReflectionVert(patterns[i]);

        if (row > 0) {
            rowsNum += row;
        }
        else if (col > 0) {
            columnsNum += col;
        }
    }

    int summary = columnsNum + 100*rowsNum; 
     
    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tSummary: %d\n", summary);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    for (int i = 0; i < patternsAmount; i++) {
        free(patterns[i].matrix);
    }
    free(patterns);
    fprintf(stdout, "\tMemory freed\n");

    fclose(fd);

    return OK;
}