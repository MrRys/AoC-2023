#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 141

typedef enum direction {
    N,  // UP
    S,  // DOWN
    E,  // RIGHT
    W   // LEFT
} Direction;

typedef enum path {
    N_S,   // |
    E_W,   // -
    N_E,   // L
    N_W,   // J
    S_W,   // 7
    S_E,   // F
    G,     // .
    START  // S
} Path;

Path get_path(char c) {
    switch (c) {
        case '|':
            return N_S;
        case '-':
            return E_W;
        case 'L':
            return N_E;
        case 'J':
            return N_W;
        case '7':
            return S_W;
        case 'F':
            return S_E;
        case '.':
            return G;
        case 'S':
            return START;
    }

    return G;
}

bool can_move(Path next, Direction dir) {
    switch (dir) {
        case N:  // UP
            switch (next) {
                case START:  // S
                case N_S:    // |
                case S_W:    // 7
                case S_E:    // F
                    return true;
                default:
                    return false;
            }
            break;
        case S:  // DOWN
            switch (next) {
                case START:  // S
                case N_S:    // |
                case N_E:    // L
                case N_W:    // J
                    return true;
                default:
                    return false;
            }
            break;
        case E:  // RIGHT
            switch (next) {
                case START:  // S
                case E_W:    // -
                case N_W:    // J
                case S_W:    // 7
                    return true;
                default:
                    return false;
            }
            break;
        case W:  // LEFT
            switch (next) {
                case START:  // S
                case E_W:    // -
                case N_E:    // L
                case S_E:    // F
                    return true;
                default:
                    return false;
            }
            break;
    }

    return false;
}

void move(Direction dir, int *col, int *row) {
    switch (dir) {
        case N:
            (*row)--;
            break;
        case S:
            (*row)++;
            break;
        case W:
            (*col)--;
            break;
        case E:
            (*col)++;
            break;
    }
}

void switch_direction(Path next, Direction *dir) {
    switch (*dir) {
        case N:  // UP
            switch (next) {
                case S_W:  // 7
                    *dir = W;
                    break;
                case S_E:  // F
                    *dir = E;
                    break;
                default:
                    break;
            }
            break;
        case S:  // DOWN
            switch (next) {
                case N_E:  // L
                    *dir = E;
                    break;
                case N_W:  // J
                    *dir = W;
                    break;
                default:
                    break;
            }
            break;
        case E:  // RIGHT
            switch (next) {
                case N_W:  // J
                    *dir = N;
                    break;
                case S_W:  // 7
                    *dir = S;
                    break;
                default:
                    break;
            }
            break;
        case W:  // LEFT
            switch (next) {
                case N_E:  // L
                    *dir = N;
                    break;
                case S_E:  // F
                    *dir = S;
                    break;
                default:
                    break;
            }
            break;
    }
}

void parse_map(char map[LEN][LEN], int *rows, int *cols) {
    *rows = 0;
    while (scanf("%s", map[*rows]) > 0) {
        (*rows)++;
    }
    *cols = strlen(map[0]);
}

void find_start(char map[LEN][LEN], int rows, int cols, int *s_row, int *s_col) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (map[row][col] == 'S') {
                *s_row = row;
                *s_col = col;
                return;
            }
        }
    }
}

void find_init_direction(char map[LEN][LEN], int rows, int cols, int row, int col, Direction *dir) {
    if (row - 1 >= 0) {
        Path north_path = get_path(map[row - 1][col]);
        if (can_move(north_path, N)) {
            *dir = N;
            return;
        }
    }

    if (row + 1 < rows) {
        Path south_path = get_path(map[row + 1][col]);
        if (can_move(south_path, S)) {
            *dir = S;
            return;
        }
    }

    if (col - 1 >= 0) {
        Path west_path = get_path(map[row][col - 1]);
        if (can_move(west_path, W)) {
            *dir = W;
            return;
        }
    }

    if (col + 1 < cols) {
        Path east_path = get_path(map[row][col + 1]);
        if (row - 1 >= 0 && can_move(east_path, E)) {
            *dir = E;
            return;
        }
    }
}

int solve(char map[LEN][LEN], int rows, int cols, int s_row, int s_col) {
    int path_length = 0;
    Direction dir;
    int curr_row = s_row, curr_col = s_col;

    find_init_direction(map, rows, cols, s_row, s_col, &dir);

    do {
        move(dir, &curr_col, &curr_row);
        switch_direction(get_path(map[curr_row][curr_col]), &dir);
        path_length++;
    } while (get_path(map[curr_row][curr_col]) != START);

    return path_length / 2;
}

void replace_start(char map[LEN][LEN], int row, int col) {
    Path north_path = get_path(map[row - 1][col]);
    Path south_path = get_path(map[row + 1][col]);
    Path east_path = get_path(map[row][col + 1]);
    Path west_path = get_path(map[row][col - 1]);

    bool can_north = can_move(north_path, N);
    bool can_south = can_move(south_path, S);
    bool can_east = can_move(east_path, E);
    bool can_west = can_move(west_path, W);

    if (can_north && can_south) {
        map[row][col] = '|';
    }

    if (can_north && can_east) {
        map[row][col] = 'L';
    }

    if (can_north && can_west) {
        map[row][col] = 'J';
    }

    if (can_south && can_east) {
        map[row][col] = 'F';
    }

    if (can_south && can_west) {
        map[row][col] = '7';
    }

    if (can_east && can_west) {
        map[row][col] = '-';
    }
}

int solve_hard(char map[LEN][LEN], int rows, int cols, int s_row, int s_col) {
    int result = 0;
    Direction dir;
    int curr_row = s_row, curr_col = s_col;
    bool path[LEN][LEN] = {0};

    find_init_direction(map, rows, cols, s_row, s_col, &dir);

    do {
        path[curr_row][curr_col] = true;
        move(dir, &curr_col, &curr_row);
        switch_direction(get_path(map[curr_row][curr_col]), &dir);
    } while (get_path(map[curr_row][curr_col]) != START);

    replace_start(map, s_row, s_col);

    for (int row = 0; row < rows; row++) {
        char prev = '.';
        int crosses = 0;

        for (int col = 0; col < cols; col++) {
            if (path[row][col]) {
                switch (map[row][col]) {
                    case '|':
                        crosses++;
                        break;
                    case '7':
                        if (prev == 'L') {
                            crosses++;
                        }
                        break;
                    case 'J':
                        if (prev == 'F') {
                            crosses++;
                        }
                        break;
                }

                if (map[row][col] != '-') {
                    prev = map[row][col];
                }

            } else if (crosses % 2) {
                result++;
            }
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    char map[LEN][LEN] = {0};
    int rows = 0, cols = 0, s_row = 0, s_col = 0;

    parse_map(map, &rows, &cols);
    find_start(map, rows, cols, &s_row, &s_col);

    int result = ishard ? solve_hard(map, rows, cols, s_row, s_col) : solve(map, rows, cols, s_row, s_col);
    printf("%d\n", result);
}
