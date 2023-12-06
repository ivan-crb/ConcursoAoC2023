#define OK 0
#define MAX 300

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
   
    unsigned long *seeds = (unsigned long *) calloc(MAX, sizeof(unsigned long));
    int seedsAmount = 0;

    char *current = (char *) calloc(MAX, sizeof(char));
    char *map = (char *) calloc(MAX, sizeof(char));
    int *boolSeeds = (int *) calloc(1, sizeof(int));
    
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
            while (!isLetter(*current) && *current != '\0') {
                unsigned long mapNums[3];
                
                int i = 0;
                while (*current != '\n' && *current != '\0') {
                    if (isDigit(*current)) mapNums[i++] = strtoul(current, &current, 10);
                    else current++;
                }
                fgets(current, MAX, fd);
                
                for (int i = 0; i < seedsAmount; i++) {
                    unsigned long seed = seeds[i];
                    
                    if (seed >= mapNums[1] && seed < (mapNums[1] + mapNums[2]) && !boolSeeds[i]) {
                        unsigned long sum = seed + mapNums[0] - mapNums[1];
                        seeds[i] = sum;
                        boolSeeds[i] = 1;
                    }
                }
            }
        } 
    }
    
    unsigned long lowestLocation = seeds[0];
    for (int i = 0; i < seedsAmount; i++) {
        if (lowestLocation > seeds[i]) lowestLocation = seeds[i];
    }

    fprintf(stdout, "Lowest location number: %lu", lowestLocation);

    free(current);

    return OK;
}