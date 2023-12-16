#define OK 0
#define MAX 12000

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct point_t {
    int x;
    int y;
    enum Direction dir;
} point;

void move(char **matrix, int width, int height, int x, int y, enum Direction dir, point *visited, int *visitedLength) {
    int isVisited = 0;
    for (int i = 0; i < *visitedLength && !isVisited; i++) {
        if (visited[i].x == x && visited[i].y == y && visited[i].dir == dir) isVisited = 1;
    }
    if (isVisited) return;
    visited[*visitedLength].x = x;
    visited[*visitedLength].y = y;
    visited[*visitedLength].dir = dir;
    (*visitedLength)++;

    char currentSquare = matrix[y][x];
    switch (dir) {
    case UP:
        switch (currentSquare) {
            case '/':
                if (x < width - 1) move(matrix, width, height, x + 1, y, RIGHT, visited, visitedLength);
                break;
            case '\\':
                if (x > 0) move(matrix, width, height, x - 1, y, LEFT, visited, visitedLength);
                break;
            case '-':
                if (x < width - 1) move(matrix, width, height, x + 1, y, RIGHT, visited, visitedLength);
                if (x > 0) move(matrix, width, height, x - 1, y, LEFT, visited, visitedLength);
                break;
            default:
                if (y > 0) move(matrix, width, height, x, y - 1, UP, visited, visitedLength);
                break;
        }
        break;
    case DOWN:
        switch (currentSquare) {
            case '/':
                if (x > 0) move(matrix, width, height, x - 1, y, LEFT, visited, visitedLength);
                break;
            case '\\':
                if (x < width - 1) move(matrix, width, height, x + 1, y, RIGHT, visited, visitedLength);
                break;
            case '-':
                if (x < width - 1) move(matrix, width, height, x + 1, y, RIGHT, visited, visitedLength);
                if (x > 0) move(matrix, width, height, x - 1, y, LEFT, visited, visitedLength);
                break;
            default:
                if (y < height - 1) move(matrix, width, height, x, y + 1, DOWN, visited, visitedLength);
                break;
        }
        break;
    case LEFT:
        switch (currentSquare) {
            case '/':
                if (y < height - 1) move(matrix, width, height, x, y + 1, DOWN, visited, visitedLength);
                break;
            case '\\':
                if (y > 0) move(matrix, width, height, x, y - 1, UP, visited, visitedLength);
                break;
            case '|':
                if (y < height - 1) move(matrix, width, height, x, y + 1, DOWN, visited, visitedLength);
                if (y > 0) move(matrix, width, height, x, y - 1, UP, visited, visitedLength);
                break;
            default:
                if (x > 0) move(matrix, width, height, x - 1, y, LEFT, visited, visitedLength);
                break;
        }
        break;

    case RIGHT:
        switch (currentSquare) {
            case '/':
                if (y > 0) move(matrix, width, height, x, y - 1, UP, visited, visitedLength);
                break;
            case '\\':
                if (y < height - 1) move(matrix, width, height, x, y + 1, DOWN, visited, visitedLength);
                break;
            case '|':
                if (y < height - 1) move(matrix, width, height, x, y + 1, DOWN, visited, visitedLength);
                if (y > 0) move(matrix, width, height, x, y - 1, UP, visited, visitedLength);
                break;
            default:
                if (x < width - 1) move(matrix, width, height, x + 1, y, RIGHT, visited, visitedLength);
                break;
        }
        break;
    }
}

int findEnergizedTiles(char **matrix, int width, int height, int x, int y, enum Direction dir) {
    point *visited = calloc(width * height, sizeof(point));   
    int visitedLength = 0;

    move(matrix, width, height, x, y, dir, visited, &visitedLength);

    point *different = calloc(visitedLength, sizeof(point));
    int diffAmount = 0;
    for (int i = 0; i < visitedLength; i++) {
        int found = 0;
        for (int j = 0; j < diffAmount && !found; j++) {
            if (visited[i].x == different[j].x && visited[i].y == different[j].y) found = 1;
        }
        if (!found) {
            different[diffAmount].x = visited[i].x;
            different[diffAmount].y = visited[i].y;
            diffAmount++;
        }
    }
    return diffAmount;
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
   
    int rowLength = 0;
    while (matrix[0][rowLength] != '\n' && matrix[0][rowLength] != '\0') rowLength++;

    int energizedTiles = findEnergizedTiles(matrix, rowLength, rowsAmount, 0, 0, RIGHT);

    fprintf(stdout, "--------------------------------------------------\n");
    fprintf(stdout, "\tEnergized tiles: %d\n", energizedTiles);
    fprintf(stdout, "\tTime: %f ms\n", ((double) clock() - startingTime) / CLOCKS_PER_SEC);
    fprintf(stdout, "--------------------------------------------------\n");

    fclose(fd);

    return OK;
}