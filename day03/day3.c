#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_LINE 150

bool is_valid(char map[MAX_LINE][MAX_LINE], int rows, int row, int num_start, int num_end) {
    int col_start = num_start - 1 < 0 ? 0 : num_start - 1;
    int col_end = (map[row][num_end + 1] == '\n' || map[row][num_end + 1] == '\0') ? num_end : num_end + 1;
    int row_start = row - 1 >= 0 ? row - 1 : row;
    int row_end = row + 1 < rows ? row + 1 : row;

    for (int r = row_start; r <= row_end; r++) {
        for (int c = col_start; c <= col_end; c++) {
            if (!isdigit(map[r][c]) && map[r][c] != '.') {
                return true;
            }
        }
    }

    return false;
}

int solve(char map[MAX_LINE][MAX_LINE], int rows) {
    int result = 0;

    for (int row = 0; row < rows; row++) {
        int num_start = -1, num_end = -1;
        for (int col = 0; col < MAX_LINE; col++) {
            if (isdigit(map[row][col])) {
                if (num_start < 0) {
                    num_start = col;
                }
                num_end = col;
            } else if (num_start != -1 && !isdigit(map[row][col])) {
                if (is_valid(map, rows, row, num_start, num_end)) {
                    int num;
                    sscanf(map[row] + num_start, "%d", &num);
                    result += num;
                }
                num_start = num_end = -1;
            }
        }
    }

    return result;
}

void connect_stars(char map[MAX_LINE][MAX_LINE], int stars[MAX_LINE * MAX_LINE][7], int rows, int row, int num_start, int num_end) {
    int col_start = num_start - 1 < 0 ? 0 : num_start - 1;
    int col_end = (map[row][num_end + 1] == '\n' || map[row][num_end + 1] == '\0') ? num_end : num_end + 1;
    int row_start = row - 1 >= 0 ? row - 1 : row;
    int row_end = row + 1 < rows ? row + 1 : row;

    for (int r = row_start; r <= row_end; r++) {
        for (int c = col_start; c <= col_end; c++) {
            if (map[r][c] == '*') {
                int num;
                sscanf(map[row] + num_start, "%d", &num);

                int star_id = r * MAX_LINE + c;
                stars[star_id][0]++;
                stars[star_id][stars[star_id][0]] = num;
            }
        }
    }
}

int solve_hard(char map[MAX_LINE][MAX_LINE], int rows) {
    int stars[MAX_LINE * MAX_LINE][7] = {0};
    int result = 0;

    for (int row = 0; row < rows; row++) {
        int num_start = -1, num_end = -1;
        for (int col = 0; col < MAX_LINE; col++) {
            if (isdigit(map[row][col])) {
                if (num_start < 0) {
                    num_start = col;
                }
                num_end = col;
            } else if (num_start != -1 && !isdigit(map[row][col])) {
                connect_stars(map, stars, rows, row, num_start, num_end);
                num_start = num_end = -1;
            }
        }
    }

    for (int star_id = 0; star_id < MAX_LINE * MAX_LINE; star_id++) {
        if (stars[star_id][0] == 2) {
            result += stars[star_id][1] * stars[star_id][2];
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int rows = 0;
    char map[MAX_LINE][MAX_LINE] = {0};

    while (fgets(map[rows], MAX_LINE, stdin) != NULL) {
        rows++;
    }

    int result = ishard ? solve_hard(map, rows) : solve(map, rows);
    printf("%d\n", result);
}
