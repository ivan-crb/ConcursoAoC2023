#define OK 0
#define MAX 12000


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


int takesFromWest(char ch) {
    return ch == 'J' || ch == '7' || ch == '-';
}

int takesFromEast(char ch) {
    return ch == 'F' || ch == 'L' || ch == '-';
}

int takesFromNorth(char ch) {
    return ch == 'J' || ch == 'L' || ch == '|';
}

int takesFromSouth(char ch) {
    return ch == 'F' || ch == '7' || ch == '|';
}

int loopLength(char **matrix, int height, int width, int x, int y, char from) {
    switch (matrix[y][x])
    {
    case 'S':
        return 1;
        break;
    case '|':
        if (from == 'n') return 1 + loopLength(matrix, height, width, x, y + 1, 'n');
        else if (from == 's') return 1 + loopLength(matrix, height, width, x, y - 1, 's');
        break;
    case '-':
        if (from == 'w') return 1 + loopLength(matrix, height, width, x + 1, y, 'w');
        else if (from == 'e') return 1 + loopLength(matrix, height, width, x - 1, y, 'e');
        break;
    case 'L':
        if (from == 'n') return 1 + loopLength(matrix, height, width, x + 1, y, 'w');
        else if (from == 'e') return 1 + loopLength(matrix, height, width, x, y - 1, 's');
        break;
    case 'J':
        if (from == 'n') return 1 + loopLength(matrix, height, width, x - 1, y, 'e');
        else if (from == 'w') return 1 + loopLength(matrix, height, width, x, y - 1, 's');
        break;
    case '7':
        if (from == 's') return 1 + loopLength(matrix, height, width, x - 1, y, 'e');
        else if (from == 'w') return 1 + loopLength(matrix, height, width, x, y + 1, 'n');
        break;
    case 'F':
        if (from == 's') return 1 + loopLength(matrix, height, width, x + 1, y, 'w');
        else if (from == 'e') return 1 + loopLength(matrix, height, width, x, y + 1, 'n');
        break;
    default:
        return 0;
        break;
    }
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

    char **lines = calloc(MAX, sizeof(char *));
    int linesAmount = 0;
    int linesLength = 0;

    while(!feof(fd)) {
        char *current = calloc(MAX, sizeof(char));
        fgets(current, MAX, fd);

        int i = 0;
        while (*(current + i) != '\n' && *(current + i) != '\0') i++;
        *(current + i) = '\0';
        linesLength = i;

        lines[linesAmount++] = current;
    } 

    int startCoordX = -1;
    int startCoordY = -1;

    for (int i = 0; i < linesAmount && startCoordY < 0; i++) {
        for (int j = 0; j < linesLength && startCoordX < 0; j++) {
            if (lines[i][j] == 'S') {
                startCoordX = j;
                startCoordY = i;
            }
        }
    }

    int distance = -1;

    if (startCoordX > 0  && startCoordX < linesLength - 1 && startCoordY > 0  && startCoordY < linesAmount - 1) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordX == 0 && startCoordY == 0) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
    }
    else if (startCoordX == 0 && startCoordY == linesAmount - 1) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordX == linesLength - 1 && startCoordY == 0) {
        if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
    }
    else if (startCoordX == linesLength - 1 && startCoordY == linesAmount - 1) {
        if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordX == 0) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordX == linesLength - 1) {
        if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordY == 0) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
    }
    else if (startCoordY == linesAmount - 1) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = loopLength(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = loopLength(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }


    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tSteps to the farthest point: %d\n", distance / 2);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    fclose(fd);

    return OK;
}