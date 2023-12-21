#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX(a, b) (a > b ? a : b)

#define MAP_SIZE 115

typedef enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

void parse_map(char map[MAP_SIZE][MAP_SIZE], int *width, int *height) {
    *width = *height = 0;

    while (scanf("%s", map[*height]) > 0) {
        (*height)++;
    }

    *width = strlen(map[0]);
}

void switch_direction(char tile, int stack[MAP_SIZE * MAP_SIZE][3], int *last, Direction *dir, int row, int col) {
    switch (tile) {
        case '/':
            switch (*dir) {
                case UP:
                    *dir = RIGHT;
                    break;
                case DOWN:
                    *dir = LEFT;
                    break;
                case LEFT:
                    *dir = DOWN;
                    break;
                case RIGHT:
                    *dir = UP;
                    break;
            }
            break;
        case '\\':
            switch (*dir) {
                case UP:
                    *dir = LEFT;
                    break;
                case DOWN:
                    *dir = RIGHT;
                    break;
                case LEFT:
                    *dir = UP;
                    break;
                case RIGHT:
                    *dir = DOWN;
                    break;
            }
            break;
        case '|':
            switch (*dir) {
                case LEFT:
                case RIGHT:
                    *dir = UP;
                    stack[*last][0] = DOWN;
                    stack[*last][1] = row;
                    stack[*last][2] = col;
                    (*last)++;
                    break;
                default:
                    break;
            }
            break;
        case '-':
            switch (*dir) {
                case UP:
                case DOWN:
                    *dir = LEFT;
                    stack[*last][0] = RIGHT;
                    stack[*last][1] = row;
                    stack[*last][2] = col;
                    (*last)++;
                    break;
                default:
                    break;
            }
            break;
    }
}

bool move(Direction dir, int width, int height, int *row, int *col) {
    switch (dir) {
        case UP:
            if (*row - 1 < 0) {
                return false;
            }
            (*row)--;
            break;
        case DOWN:
            if (*row + 1 >= height) {
                return false;
            }
            (*row)++;
            break;
        case LEFT:
            if (*col - 1 < 0) {
                return false;
            }
            (*col)--;
            break;
        case RIGHT:
            if (*col + 1 >= width) {
                return false;
            }
            (*col)++;
            break;
    }
    return true;
}

int count_energized(char light_map[MAP_SIZE][MAP_SIZE], int width, int height) {
    int count = 0;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (light_map[row][col] == '#') {
                count++;
            }
        }
    }

    return count;
}

int solve(char mirror_map[MAP_SIZE][MAP_SIZE], int width, int height, Direction start_dir, int start_row, int start_col) {
    char light_map[MAP_SIZE][MAP_SIZE] = {0};
    bool dir_map[MAP_SIZE][MAP_SIZE][4] = {0};

    int last = 0;
    int stack[MAP_SIZE * MAP_SIZE][3] = {0};
    stack[last][0] = start_dir;
    stack[last][1] = start_row;
    stack[last][2] = start_col;
    last++;

    while (last > 0) {
        last--;

        Direction dir = stack[last][0];
        int row = stack[last][1];
        int col = stack[last][2];

        while (!dir_map[row][col][dir]) {
            light_map[row][col] = '#';
            dir_map[row][col][dir] = true;

            switch_direction(mirror_map[row][col], stack, &last, &dir, row, col);

            if (!move(dir, width, height, &row, &col)) {
                break;
            }
        }
    }

    return count_energized(light_map, width, height);
}

int solve_hard(char mirror_map[MAP_SIZE][MAP_SIZE], int width, int height) {
    int max_energized = 0;

    for (int col = 0; col < width; col++) {
        max_energized = MAX(max_energized, solve(mirror_map, width, height, DOWN, 0, col));
        max_energized = MAX(max_energized, solve(mirror_map, width, height, UP, height - 1, col));
    }

    for (int row = 0; row < height; row++) {
        max_energized = MAX(max_energized, solve(mirror_map, width, height, RIGHT, row, 0));
        max_energized = MAX(max_energized, solve(mirror_map, width, height, LEFT, row, width - 1));
    }

    return max_energized;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int width = 0, height = 0;
    char mirror_map[MAP_SIZE][MAP_SIZE] = {0};

    parse_map(mirror_map, &width, &height);

    int result = ishard ? solve_hard(mirror_map, width, height) : solve(mirror_map, width, height, RIGHT, 0, 0);
    printf("%d\n", result);
}
