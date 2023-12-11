#define OK 0
#define MAX 12000


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct coords_t {
    int row;
    int col;
} coord;


int contains(char *arr, char ch) {
    int contained = 0;
    for (int i = 0; arr[i] != '\n' && arr[i] != '\0' && !contained; i++) {
        if (arr[i] == ch) contained = 1;
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

    for (int i = 0; i < rowsAmount; i++) {
        if (!contains(matrix[i], '#')) {
            for (int j = rowsAmount; j > i; j--) {
                matrix[j] = matrix[j - 1];
            }
            rowsAmount++;
            i++;
        }
    }

    for (int i = 0; i < rowLength; i++) {
        int contains = 0;
        for (int j = 0; j < rowsAmount && !contains; j++) {
            if (matrix[j][i] == '#') contains = 1;
        }

        if (!contains) {
            for (int j = 0; j < rowsAmount; j++) {
                for (int k = rowLength; k > i; k--) {
                    matrix[j][k] = matrix[j][k - 1];
                }
                matrix[j][rowLength + 1] = '\n';
            }
            rowLength++;
            i++;
        }
    }

    coord *galaxies = calloc(MAX, sizeof(coord));
    int galaxiesLength = 0;

    for (int i = 0; i < rowsAmount; i++) {
        for (int j = 0; j < rowLength; j++) {
            if (matrix[i][j] == '#') {
                coord newGal = {i, j};
                galaxies[galaxiesLength++] = newGal;
            }
        }
    }

    int shortestPathSum = 0;

    for (int i = 0; i < galaxiesLength; i++) {
        for (int j = i; j < galaxiesLength; j++) {
            int vertSteps = abs(galaxies[i].row - galaxies[j].row);
            int horzSteps = abs(galaxies[i].col - galaxies[j].col);
            int shortestPath = vertSteps + horzSteps;
            shortestPathSum += shortestPath;
        }
    }

    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tSum of shortest paths: %d\n", shortestPathSum);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    fclose(fd);

    return OK;
}