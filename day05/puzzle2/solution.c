#define OK 0
#define MAX 10000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int isLetter(char c) {
    return c >= 'a' && c <= 'z';
}

int copyTitle(char* destination, char* source) {
    int i = 0;
    while ( source[i] != '\0' && source[i] != ':') {
        destination[i] = source[i];
        i++;
    }
    return i;
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
   
    unsigned long *seeds = (unsigned long *) calloc(MAX, sizeof(unsigned long));
    unsigned long seedsAmount = 0;

    char *current = (char *) calloc(MAX, sizeof(char));
    char *map = (char *) calloc(MAX, sizeof(char));
    int *boolSeeds = (int *) calloc(1, sizeof(int));

    unsigned long count = 0;

    fgets(current, MAX, fd);
    while(!feof(fd)) {
        while (!isLetter(*current)) fgets(current, MAX, fd);

        memset(map, 0, MAX * sizeof(char));
        copyTitle(map, current);

        if (!strcmp(map, "seeds")) {
            while (*current != '\n') {
                if (isDigit(*current)) seeds[seedsAmount++] = strtoul(current, &current, 10);
                else current++;
            }
        }
        else {
            boolSeeds = (int *) realloc(boolSeeds, seedsAmount * sizeof(int));
            if (boolSeeds == NULL) printf("Error\n");
            memset(boolSeeds, 0, seedsAmount * sizeof(int));

            fgets(current, MAX, fd);
            while (!isLetter(*current) && *current != '\0' && *current != '\n') {
                unsigned long mapNums[3];

                int i = 0;
                while (*current != '\n' && *current != '\0') {
                    if (isDigit(*current)) mapNums[i++] = strtoul(current, &current, 10);
                    else current++;
                }
                fgets(current, MAX, fd);

                for (int i = 0; i < seedsAmount; i += 2) {
                    
                    unsigned long seedMin = seeds[i];
                    unsigned long seedMax = seeds[i] + seeds[i + 1] - 1;

                    unsigned long mapMin = mapNums[1];
                    unsigned long mapMax = mapNums[1] + mapNums[2] - 1;

                    if (seedMin >= mapMin && seedMax <= mapMax && !boolSeeds[i]) {
                        seeds[i] = seeds[i] + mapNums[0] - mapNums[1];
                        boolSeeds[i] = 1;
                    }
                    else if (seedMin >= mapMin && seedMin <= mapMax && seedMax >= mapMax && !boolSeeds[i]) {
                        
                        seeds[i] = seeds[i] + mapNums[0] - mapNums[1];
                        seeds[i + 1] = mapMax - seedMin + 1;
                        boolSeeds[i] = 1;
                        
                        seeds[seedsAmount++] = mapMax + 1;
                        seeds[seedsAmount++] = seedMax - mapMax;
                        boolSeeds = (int *) realloc(boolSeeds, seedsAmount * sizeof(int));
                        if (boolSeeds == NULL) printf("Error\n");
                        boolSeeds[seedsAmount - 2] = 0;
                    }
                    else if (seedMin <= mapMin && seedMax <= mapMax && seedMax >= mapMin && !boolSeeds[i]) {
                        seeds[i] = seedMin;
                        seeds[i + 1] = mapMin - seedMin;

                        seeds[seedsAmount++] = seedMin + mapMin - seedMin + mapNums[0] - mapNums[1];
                        seeds[seedsAmount++] = seedMax - mapMin + 1;   // 92 - 56
                        boolSeeds = (int *) realloc(boolSeeds, seedsAmount * sizeof(int));
                        if (boolSeeds == NULL) printf("Error\n");
                        boolSeeds[seedsAmount - 2] = 1;
                    }
                    else if (seedMin <= mapMin && seedMax >= mapMax && !boolSeeds[i]) {
                        seeds[i] = mapNums[0];
                        seeds[i + 1] = mapNums[2];
                        boolSeeds[i] = 1;

                        seeds[seedsAmount++] = seedMin;
                        seeds[seedsAmount++] = mapMin - seedMin;
                        boolSeeds = (int *) realloc(boolSeeds, seedsAmount * sizeof(int));
                        if (boolSeeds == NULL) printf("Error\n");
                        boolSeeds[seedsAmount - 2] = 0;   

                        seeds[seedsAmount++] = mapMax + 1;
                        seeds[seedsAmount++] = seedMax - mapMax;
                        boolSeeds = (int *) realloc(boolSeeds, seedsAmount * sizeof(int));
                        if (boolSeeds == NULL) printf("Error\n");
                        boolSeeds[seedsAmount - 2] = 0;   

                    }      
                }   
            }
        } 
    }

    unsigned long lowestLocation = seeds[0];

    for (int i = 0; i < seedsAmount; i += 2) {
        if (lowestLocation > seeds[i]) lowestLocation = seeds[i];
    }

    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tLowest location number: %lu\n", lowestLocation);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    free(current);

    return OK;
}