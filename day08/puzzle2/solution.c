#define OK 0
#define MAX 12000
#define NODELENGTH 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct keyPair_t {
    char *key;
    char *left;
    char *right;
} keyPair;

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ;
}

int isSymbol(char c) {
    return isDigit(c) || isLetter(c);
}

keyPair findKey (keyPair *arr, int arrLength, char *elem) {
    int pos = -1;
    for (int i = 0; i < arrLength && pos < 0; i++) {
        if (strcmp(arr[i].key, elem) == 0) pos = i;
    }
    return arr[pos];
}


long long mcd(long long a, long long b) {
    if (b == 0) return a;
    return mcd(b, a % b);
}

long long lcm(long long a, long long b) {
    return (a * b) / mcd(a, b);
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

    char *instructions = calloc(MAX, sizeof(char));
    int instructionsLength = 0;
    keyPair *bimap = calloc(MAX, sizeof(keyPair));
    int bimapLength = 0;
    char *current = calloc(MAX, sizeof(char));
    
    fgets(current, MAX, fd);
    while (*current == 'L' || *current == 'R') {
        instructions[instructionsLength++] = *current;
        current++;
    }

    while(!feof(fd)) {
        while (!isSymbol(*current)) fgets(current, MAX, fd);

        bimap[bimapLength].key = calloc(NODELENGTH + 1, sizeof(char));
        bimap[bimapLength].left = calloc(NODELENGTH + 1, sizeof(char));
        bimap[bimapLength].right = calloc(NODELENGTH + 1, sizeof(char));

        for (int i = 0; i < NODELENGTH; i++) {
            bimap[bimapLength].key[i] = *current;
            current++;
        }
        bimap[bimapLength].key[NODELENGTH] = '\0';

        while (!isSymbol(*current)) current++;

        for (int i = 0; i < NODELENGTH; i++) {
            bimap[bimapLength].left[i] = *current;
            current++;
        }
        bimap[bimapLength].left[NODELENGTH] = '\0';
        
        while (!isSymbol(*current)) current++;

        for (int i = 0; i < NODELENGTH; i++) {
            bimap[bimapLength].right[i] = *current;
            current++;
        }
        bimap[bimapLength].right[NODELENGTH] = '\0';
        
        bimapLength++;
    } 

    keyPair *firstNodes = calloc(MAX, sizeof(keyPair));
    int pastNodesLength = 0;
    
    for (int i = 0; i < bimapLength; i++) {
        if (bimap[i].key[NODELENGTH - 1] == 'A') {
            firstNodes[pastNodesLength++] = bimap[i];
        }
    }

    unsigned int *stepsArr = calloc(pastNodesLength, sizeof(unsigned int));

    for (int i = 0; i < pastNodesLength; i++) {
        unsigned int steps = 0;
        keyPair currentNode = firstNodes[i];
        
        while (currentNode.key[NODELENGTH - 1] != 'Z') {
            switch (instructions[steps % instructionsLength]) {
                case 'L':
                    currentNode = findKey(bimap, bimapLength, currentNode.left);
                    break;
                case 'R':
                    currentNode = findKey(bimap, bimapLength, currentNode.right);
                    break;
                }
            steps++;
        }
        stepsArr[i] = steps;
    }

    long long totalSteps = 1;
    
    for (int i = 0; i < pastNodesLength; i++) {
        totalSteps = lcm(totalSteps, stepsArr[i]);
    }

    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tSteps: %lld\n", totalSteps);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    free(current);
    printf("memory freed\n");

    return OK;
}