#define OK 0
#define MAX 12000
#define NODELENGTH 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

keyPair findKey (keyPair *arr, int arrLength, char *elem) {
    int pos = -1;
    for (int i = 0; i < arrLength && pos < 0; i++) {
        if (strcmp(arr[i].key, elem) == 0) pos = i;
    }
    return arr[pos];
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
        while (!isLetter(*current)) fgets(current, MAX, fd);

        bimap[bimapLength].key = calloc(NODELENGTH + 1, sizeof(char));
        bimap[bimapLength].left = calloc(NODELENGTH + 1, sizeof(char));
        bimap[bimapLength].right = calloc(NODELENGTH + 1, sizeof(char));

        for (int i = 0; i < NODELENGTH; i++) {
            bimap[bimapLength].key[i] = *current;
            current++;
        }
        bimap[bimapLength].key[NODELENGTH] = '\0';

        while (!isLetter(*current)) current++;

        for (int i = 0; i < NODELENGTH; i++) {
            bimap[bimapLength].left[i] = *current;
            current++;
        }
        bimap[bimapLength].left[NODELENGTH] = '\0';
        
        while (!isLetter(*current)) current++;

        for (int i = 0; i < NODELENGTH; i++) {
            bimap[bimapLength].right[i] = *current;
            current++;
        }
        bimap[bimapLength].right[NODELENGTH] = '\0';
        
        bimapLength++;
    } 
    
    unsigned int steps = 0;
    keyPair currentNode = findKey(bimap, bimapLength, "AAA\0");
    
    while (strcmp(currentNode.key, "ZZZ\0") != 0) {
        switch (instructions[steps % instructionsLength])
        {
        case 'L':
            currentNode = findKey(bimap, bimapLength, currentNode.left);
            break;
        
        case 'R':
            currentNode = findKey(bimap, bimapLength, currentNode.right);
            break;
        }
        steps++;
    }

    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tSteps: %lu\n", steps);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    free(current);
    printf("memory freed\n");

    return OK;
}