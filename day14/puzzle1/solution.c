#define OK 0
#define MAX 12000

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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
        for (int j = 0; j < rowLength; j++) {
            if (matrix[i][j] == 'O') {
                for (int k = i; k > 0 && matrix[k-1][j] == '.'; k--) {
                    matrix[k-1][j] = 'O';
                    matrix[k][j] = '.';
                }
            }
        }
    }

    int totalLoad = 0;

    for (int i = 0; i < rowsAmount; i++) {
        for (int j = 0; j < rowLength; j++) {
            if (matrix[i][j] == 'O') {
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