#define OK 0
#define MAX 12000
#define COPIES 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int count (char *arr, char ch) {
    int count = 0;
    for (int i = 0; arr[i] != '\n' && arr[i] != '\0'; i++) {
        if (arr[i] == ch) count++;
    }
    return count;
}

int countContiguous (char *arr, char ch, int from, int *to) {
    int i = from;
    int count = 0;
    int allCorrect = 1;
    while (allCorrect && arr[i] != '\n' && arr[i] != '\0') {
        if (arr[i] == ch) {
            count++;
            i++;
        }
        else allCorrect = 0;
    }
    *to = i;
    return count;
}

int copy (char *src, char * dest) {
    int i = 0;
    while (src[i] != '\n' && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int replaceFirst (char *arr, char from, char to) {
    int found = 0;
    for (int i = 0; arr[i] != '\0' && arr[i] != '\n' && !found; i++) {
        if (arr[i] == from) {
            arr[i] = to;
            found = 1;
        }
    }
}

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

    clock_t startingTime = clock();

    char **matrix = calloc(MAX, sizeof(char *));
    int rowsAmount = 0;

    while(!feof(fd)) {
        char *row = calloc(MAX, sizeof(char));
        fgets(row, MAX, fd);
        matrix[rowsAmount++] = row;
    }

    long long arrangementSum = 0;

    for (int i = 0; i < rowsAmount; i++) {
        
        char *springs = calloc(MAX, sizeof(char));
        char *numbers = calloc(MAX, sizeof(char));

        int desp = 0;
        while (matrix[i][desp] != ' ') {
            //printf(" c: %c\n", matrix[i][desp]);
            springs[desp] = matrix[i][desp];
            desp++;
        }
        //printf("Springs %s\n", springs);
        desp++;
        int desp2 = 0;
        //printf(" c: %c\n", matrix[i][desp]);
        while (matrix[i][desp] != '\n' && matrix[i][desp] != '\0') {
            //printf(" c: \n", matrix[i][desp]);
            numbers[desp2] = matrix[i][desp];
            desp++;
            desp2++;
        }
        //printf("Numbers %s\n", numbers);

        strcpy(matrix[i], springs);
        for (int j = 1; j < COPIES; j++) {
            strcat(matrix[i], "?");
            strcat(matrix[i], springs);
        }
        strcat(matrix[i], " ");
        strcat(matrix[i], numbers);
        for (int j = 1; j < COPIES; j++) {
            strcat(matrix[i], ",");
            strcat(matrix[i], numbers);
        }

        //printf("%s\n", matrix[i]);
        int *nums = calloc(MAX, sizeof(int));
        int numAmount = 0;
        int totalSprings = 0;
        char *aux = matrix[i];
        while (*aux != '\0' && *aux != '\n') {
            while (!isDigit(*aux)) aux++;

            int number = strtol(aux, &aux, 10);
            nums[numAmount++] = number;
            totalSprings += number;
        }

        //printf("Springs %d\n", totalSprings);
        int arrangements = 0;

        int unknownAmount = count(matrix[i], '?');
        //printf("\nUnknowns: %d\n", unknownAmount);
        
        for (int j = 0; j < pow(2, unknownAmount); j++) {

            char *arrangement = calloc(MAX, sizeof(char));
            copy(matrix[i], arrangement); 

            for (int k = unknownAmount - 1; k >= 0; k--) {
                int aux = pow(2, k);
                int pos = (j / aux) % 2;
                if (pos) replaceFirst(arrangement, '?', '.');
                else replaceFirst(arrangement, '?', '#');
            }

            int springCount = count(arrangement, '#');

            if (springCount == totalSprings) {
            //printf("%s ", arrangement);

            int *chunks = calloc(MAX, sizeof(int));
            int chunksAmount = 0;

            for (int k = 0; arrangement[k] != '\0' && arrangement[k] != '\n'; k++) {
                int count = countContiguous(arrangement, '#', k, &k);
                if (count > 0) chunks[chunksAmount++] = count;   
            }
            
            int isCorrect = 1;
            if (chunksAmount != numAmount) isCorrect = 0;
            else {
                for (int k = 0; k < chunksAmount && isCorrect; k++) {
                    if (chunks[k] != nums[k]) isCorrect = 0;
                }
            }

            //printf(" - isCorrect %d\n", isCorrect);

            if (isCorrect) arrangements++;

           
            free(chunks);
            }
            free(arrangement);
            
        }

        //printf("\n Arrangements: %d", arrangements);
        free(springs);
        free(numbers);
        free(nums);

        arrangementSum += arrangements;
    }
    
    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tSum of possible arrangements: %lld\n", arrangementSum);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    for (int i = 0; i < rowsAmount; i++) {
        free(matrix[i]);
    }
    free(matrix);
    fprintf(stdout, "\tMemory freed\n");

    fclose(fd);

    return OK;
}