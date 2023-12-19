#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MIN(a, b) (a < b ? a : b)
#define MAX_SIZE 20

bool ishard = false;

int strdiff(char *str1, char *str2) {
    int str1_len = strlen(str1);
    int diff = 0;
    for (int i = 0; i < str1_len; i++) {
        if (str1[i] != str2[i]) {
            diff++;
        }
    }
    return diff;
}

void tilt_mirror(char original[MAX_SIZE][MAX_SIZE], char tilted[MAX_SIZE][MAX_SIZE], int width, int height) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            tilted[col][row] = original[row][col];
        }
    }
}

bool is_perfect(char mirror[MAX_SIZE][MAX_SIZE], int height, int start) {
    int diff = 0;

    for (int i = 0; start + i + 1 < height && start - i >= 0; i++) {
        diff += strdiff(mirror[start + i + 1], mirror[start - i]);
    }

    return diff == ishard;
}

int find_horizontal_reflexion(char mirror[MAX_SIZE][MAX_SIZE], int height) {
    for (int row = 0; row < height - 1; row++) {
        if (strdiff(mirror[row], mirror[row + 1]) > ishard) {
            continue;
        }

        if (is_perfect(mirror, height, row)) {
            return row + 1;
        }
    }

    return -1;
}

int find_vertical_reflexion(char mirror[MAX_SIZE][MAX_SIZE], int width, int height) {
    char tilted_mirror[MAX_SIZE][MAX_SIZE] = {0};
    tilt_mirror(mirror, tilted_mirror, width, height);
    return find_horizontal_reflexion(tilted_mirror, width);
}

int solve(char mirror[MAX_SIZE][MAX_SIZE], int width, int height) {
    int vertical_reflexion = find_vertical_reflexion(mirror, width, height);
    if (vertical_reflexion > 0) {
        return vertical_reflexion;
    }

    int horizontal_reflexion = find_horizontal_reflexion(mirror, height);
    if (horizontal_reflexion > 0) {
        return horizontal_reflexion * 100;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    ishard = argc > 1 && argv[1][0] == 'h';

    int result = 0;
    int width = 0, height = 0;
    char line[MAX_SIZE] = {0};
    char mirror[MAX_SIZE][MAX_SIZE] = {0};
    char *read = NULL;

    do {
        read = fgets(line, MAX_SIZE, stdin);

        if (read != NULL && line[0] != '\n') {
            sscanf(line, "%s", mirror[height++]);
            continue;
        }

        width = strlen(mirror[0]);
        result += solve(mirror, width, height);

        width = height = 0;
        memset(mirror, 0, MAX_SIZE * MAX_SIZE);

    } while (read != NULL);

    printf("%d\n", result);
}
