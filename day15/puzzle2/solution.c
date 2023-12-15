#define OK 0
#define MAX 25000
#define BOXES 256
#define ll long long

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct lens_t {
    char *label;
    int focalLength;
} lens;

typedef struct box_t {
    lens *lenses;
    int lensesAmount;
} box;


int getHash(char *text) {
    int i = 0;
    int hash = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        hash += text[i];
        hash *= 17;
        hash = hash % 256;
    }
    return hash;
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


    char *line = calloc(MAX, sizeof(char *));
    fgets(line, MAX, fd);

    box *boxes = calloc(BOXES, sizeof(box));
    for (int i = 0; i < BOXES; i++) {
        boxes[i].lenses = calloc(MAX, sizeof(lens));
    }

    while (*line != '\0') {

        char *word = calloc(MAX, sizeof(char));
        int wordLength = 0;
        while (*line != '=' && *line != '-' && *line != '\0' && *line != '\n') {
            word[wordLength++] = *line;
            line++;
        }
        word[wordLength] = '\0';
        int hash = getHash(word);

        box *currentBox = boxes + hash;
        int pos = -1;
        for (int i = 0; i < currentBox->lensesAmount && pos < 0; i++) {
            if (strcmp(currentBox->lenses[i].label, word) == 0) pos = i;
        }
        switch (*line) {
            case '-':
                line++;
                if (pos >= 0) {
                    for (int i = pos; i < currentBox->lensesAmount - 1; i++) {
                        currentBox->lenses[i] = currentBox->lenses[i + 1];
                    }
                    currentBox->lensesAmount--;
                }
                break;
            case '=':
                line++;
                int focalLength = strtol(line, &line, 10);

                if (pos < 0) {
                    currentBox->lenses[currentBox->lensesAmount].label = word;
                    currentBox->lenses[currentBox->lensesAmount].focalLength = focalLength;
                    currentBox->lensesAmount++;
                }
                else {
                    currentBox->lenses[pos].focalLength = focalLength;
                }
                break;
        }
        line++;
    }

    int focusingPower = 0;

    for (int i = 0; i < BOXES; i++) {
        box currentBox = boxes[i];
        for (int j = 0; j < currentBox.lensesAmount; j++) {
            focusingPower += (i+1) * (j+1) * currentBox.lenses[j].focalLength;
        }
    }

    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tTotal load: %d\n", focusingPower);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    fclose(fd);

    return OK;
}