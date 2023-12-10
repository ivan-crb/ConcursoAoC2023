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

int markLoop(char **matrix, int height, int width, int x, int y, char from) {
    switch (matrix[y][x]) {
    case 'S':
        unsigned int up = 0;
        unsigned int down = 0;
        unsigned int left = 0;
        unsigned int right = 0;

        if (y > 0) up = matrix[y - 1][x] + 256;
        if (y < height - 1) down = matrix[y + 1][x] + 256;
        if (x > 0) left = matrix[y][x - 1] + 256;
        if (x < width - 1) right = matrix[y][x + 1] + 256;
        printf("down: %u | right: %u \n", down, right);

        if ( (up == 179 || up == 191 || up == 218) && (down == 179 || down == 192 || down == 217) ) matrix[y][x] = 179;
        else if ( (left == 196 || left == 192 || left == 218) && (right == 196 || right == 191 || right == 217) ) matrix[y][x] = 196;
        else if ( (up == 179 || up == 191 || up == 218) && (right == 196 || right == 191 || right == 217) ) matrix[y][x] = 192;
        else if ( (up == 179 || up == 191 || up == 218) && (left == 196 || left == 192 || left == 218) ) matrix[y][x] = 217;
        else if ( (down == 179 || down == 192 || down == 217) && (left == 196 || left == 192 || left == 218)) matrix[y][x] = 191;
        else if ( (down == 179 || down == 192 || down == 217) && (right == 196 || right == 191 || right == 217)) matrix[y][x] = 218;
        // L J 7 F
        return 1;
        break;
    case '|':
        matrix[y][x] = 179;
        if (from == 'n') return 1 + markLoop(matrix, height, width, x, y + 1, 'n');
        else if (from == 's') return 1 + markLoop(matrix, height, width, x, y - 1, 's');
        break;
    case '-':
        matrix[y][x] = 196;
        if (from == 'w') return 1 + markLoop(matrix, height, width, x + 1, y, 'w');
        else if (from == 'e') return 1 + markLoop(matrix, height, width, x - 1, y, 'e');
        break;
    case 'L':
        matrix[y][x] = 192;
        if (from == 'n') return 1 + markLoop(matrix, height, width, x + 1, y, 'w');
        else if (from == 'e') return 1 + markLoop(matrix, height, width, x, y - 1, 's');
        break;
    case 'J':
        matrix[y][x] = 217;
        if (from == 'n') return 1 + markLoop(matrix, height, width, x - 1, y, 'e');
        else if (from == 'w') return 1 + markLoop(matrix, height, width, x, y - 1, 's');
        break;
    case '7':
        matrix[y][x] = 191;
        if (from == 's') return 1 + markLoop(matrix, height, width, x - 1, y, 'e');
        else if (from == 'w') return 1 + markLoop(matrix, height, width, x, y + 1, 'n');
        break;
    case 'F':
        matrix[y][x] = 218;
        if (from == 's') return 1 + markLoop(matrix, height, width, x + 1, y, 'w');
        else if (from == 'e') return 1 + markLoop(matrix, height, width, x, y + 1, 'n');
        break;
    }
}

void markOutside(char **matrix, int height, int width, int x, int y) {
    if (matrix[y][x] == 'S' || matrix[y][x] < 0 || matrix[y][x] == ' ') return;

    matrix[y][x] = ' ';
    if (x > 0) markOutside(matrix, height, width, x - 1, y);
    if (y > 0) markOutside(matrix, height, width, x, y - 1);
    if (x < width - 1) markOutside(matrix, height, width, x + 1, y);
    if (y < height - 1) markOutside(matrix, height, width, x, y + 1);
}

int isEdge(char *line, int length, int pos) {
    unsigned int ch = line[pos] + 256;
    if (ch == 179) return 1;
    else if (ch == 192) {
        int isCorrect = 1;
        for (int i = pos + 1; i < length && isCorrect; i++) {
            unsigned int nch = line[i] + 256;
            if (nch != 196) isCorrect = 0;
            if (nch == 191) return 1;
        }
    }
    else if (ch == 218) {
        int isCorrect = 1;
        for (int i = pos + 1; i < length && isCorrect; i++) {
            unsigned int nch = line[i] + 256;
            if (nch != 196) isCorrect = 0;
            if (nch == 217) return 1;
        }
    }
    return 0;
}

int isOutside(char **matrix, int height, int width, int x, int y) {
    int intersections = 0;
    for (int i = x; i < width; i++) {
        if (isEdge(matrix[y], width, i)) intersections++;
    }
    return intersections % 2 == 0;
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
    printf("Lines: %dx%d\n", linesAmount, linesLength);
    for (int i = 0; i < linesAmount; i++) {
        printf("%s\n", lines[i]);
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
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordX == 0 && startCoordY == 0) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
    }
    else if (startCoordX == 0 && startCoordY == linesAmount - 1) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordX == linesLength - 1 && startCoordY == 0) {
        if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
    }
    else if (startCoordX == linesLength - 1 && startCoordY == linesAmount - 1) {
        if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordX == 0) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordX == linesLength - 1) {
        if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }
    else if (startCoordY == 0) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromNorth(lines[startCoordY + 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY + 1, 'n');
    }
    else if (startCoordY == linesAmount - 1) {
        if (takesFromWest(lines[startCoordY][startCoordX + 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX + 1, startCoordY, 'w');
        else if (takesFromEast(lines[startCoordY][startCoordX - 1])) distance = markLoop(lines, linesAmount, linesLength, startCoordX - 1, startCoordY, 'e');
        else if (takesFromSouth(lines[startCoordY - 1][startCoordX])) distance = markLoop(lines, linesAmount, linesLength, startCoordX, startCoordY - 1, 's');
    }

    printf("\n");
    for (int i = 0; i < linesAmount; i++) {
        printf("%s\n", lines[i]);
    }

    for (int i = 0; i < linesAmount; i++) {
        markOutside(lines, linesAmount, linesLength, 0, i);
        markOutside(lines, linesAmount, linesLength, linesLength - 1, i);
    }

    for (int i = 0; i < linesLength; i++) {
        markOutside(lines, linesAmount, linesLength, i, 0);
        markOutside(lines, linesAmount, linesLength, i, linesAmount - 1);  
    }

    int area = 0;

    for (int i = 0; i < linesAmount; i++) {
        for (int j = 0; j < linesLength; j++) {
            if (lines[i][j] > 0 && lines[i][j] != 'S' && lines[i][j] != ' ' && isOutside(lines, linesAmount, linesLength, j, i)) {
                lines[i][j] = 'O';
            }
            else if (lines[i][j] > 0 && lines[i][j] != 'S' && lines[i][j] != ' ') {
                lines[i][j] = 'I';
                area++;
            }
        }
    }

    printf("\n");
    for (int i = 0; i < linesAmount; i++) {
        printf("%s\n", lines[i]);
    }

    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tArea enclosed: %d\n", area);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    fclose(fd);

    return OK;
}