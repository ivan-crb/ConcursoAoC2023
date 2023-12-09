#define OK 0
#define MAX 12000


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef long long ll;

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int allZeros(ll *arr, int arrLength) {
    int allZero = 1;
    for (int i = 0; i < arrLength && allZero; i++) {
        if (arr[i]) allZero = 0;
    }
    return allZero;
}

void reverse(ll *arr, int arrLength) {
    ll *aux = calloc(arrLength, sizeof(ll));
    for (int i = 0; i < arrLength; i++) {
        aux[i] = arr[arrLength - 1 - i];
    }
    for (int i = 0; i < arrLength; i++){
        arr[i] = aux[i];
    }
    free(aux);
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

    ll predictionSum = 0;

    while(!feof(fd)) {
        char *current = calloc(MAX, sizeof(char));
        fgets(current, MAX, fd);

        ll **valuesLists = calloc(MAX, sizeof(ll));
        int listsLength = 0;

        ll *initValues = calloc(MAX, sizeof(ll));
        int initValuesLength = 0;
        while (*current != '\n' && *current != '\0') {
            if (isDigit(*current) || *current == '-') initValues[initValuesLength++] = strtoll(current, &current, 10);
            else current++;
        }

        reverse(initValues, initValuesLength);
        valuesLists[listsLength++] = initValues;

        for (int i = 0; !allZeros(valuesLists[i], initValuesLength - i); i++) {
            ll *newValues = calloc(MAX, sizeof(ll));
            for (int j = 0; j < initValuesLength - i - 1; j++) {
                newValues[j] = valuesLists[i][j+1] - valuesLists[i][j];
            }
            valuesLists[listsLength++] = newValues;            
        }

        for (int i = listsLength - 2; i >= 0; i--) {
            valuesLists[i][initValuesLength - i] = valuesLists[i][initValuesLength - i - 1] + valuesLists[i+1][initValuesLength - i - 1];
        }
        initValuesLength++;

        predictionSum += valuesLists[0][initValuesLength - 1];
    } 

    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tSum of predictions: %lld\n", predictionSum);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    fclose(fd);

    return OK;
}