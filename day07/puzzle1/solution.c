#define OK 0
#define MAX 12000
#define CARDS 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int count(char *arr, char c, int length) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (arr[i] == c) count++;
    }
    return count;
}

int maxReps(char *arr, int length) {
    int max = -1;
    char maxC = '\0';
    for (int i = 0; i < length; i++) {
        int reps = count(arr, arr[i], length);
        if (reps > max) {
            max = reps;
            maxC = arr[i];
        }
    }
    return max;
}

int compare(char *arr1, char *arr2, char *order) {
    int i = 0;
    while (arr1[i] == arr2[i] && arr1[i] != '\0') i++;
    int found = 0;
    for (int j = 0; order[j] != '\0' && found == 0; j++) {
        if (arr1[i] == order[j]) found = 1;
        else if (arr2[i] == order[j]) found = -1;
    }
    return found;
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
   
    char *order = {"AKQJT98765432\0"};
    char **handSet = calloc(MAX, sizeof(char *));
    long handAmount = 0;
    int *diffs = calloc(MAX, sizeof(int));
    int *bidings = calloc(MAX, sizeof(int));

    char *current = calloc(MAX, sizeof(char));
    
    while(!feof(fd)) {
        fgets(current, MAX, fd);

        char *hand = calloc(CARDS, sizeof(char));
        for (int i = 0; i < CARDS; i++) {
            hand[i] = *current;
            if (count(hand, hand[i], i) == 0) diffs[handAmount]++;
            current++;
        }
        handSet[handAmount] = hand;

        while (!isDigit(*current)) current++;
        bidings[handAmount] = strtol(current, &current, 10);

        handAmount++;  
    } 
    
    int *strength = calloc(MAX, sizeof(int));
    for (int i = 0; i < handAmount; i++) {
        char *hand = handSet[i];

        int maxRep = maxReps(hand, CARDS);
        if (maxRep == 5) {
            strength[i] = 7;
        }
        else if (maxRep == 4) {
            strength[i] = 6;
        }
        else if (maxRep == 3 && diffs[i] == 2) {
            strength[i] = 5;
        }
        else if (maxRep == 3 && diffs[i] == 3) {
            strength[i] = 4;
        }
        else if (maxRep == 2 && diffs[i] == 3) {
            strength[i] = 3;
        }
        else if (maxRep == 2 && diffs[i] == 4) {
            strength[i] = 2;
        }
        else {
            strength[i] = 1;
        }
    }

    int *checked = calloc(MAX, sizeof(int));
    unsigned long totalWinnings = 0;
    
    for (int i = handAmount; i > 0; i--) {
        char *maxHand = handSet[0];
        int maxPos = -1;
        int maxStrength = -1;
        for (int j = 0; j < handAmount; j++) {
            if (strength[j] > maxStrength && !checked[j]) {
                maxHand = handSet[j];
                maxPos = j;
                maxStrength = strength[j];
            }
            else if (strength[j] == maxStrength && compare(handSet[j], maxHand, order) >= 0 && !checked[j]) {
                maxHand = handSet[j];
                maxPos = j;
            }
        }

        totalWinnings += i * bidings[maxPos];
        checked[maxPos] = 1;
    }
    
    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tTotal winnings: %lu\n", totalWinnings);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    free(current);
    printf("memory freed\n");

    return OK;
}