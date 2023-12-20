#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 105
#define MAX_LOAD 100000
#define MAX_CYCLES 300
#define HARD_CYCLE 1000000000

void parse_dish(char dish[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    *rows = *cols = 0;

    while (scanf("%s", dish[*rows]) > 0) {
        (*rows)++;
    }

    *cols = strlen(dish[0]);
}

void copy_dish(char original[MAX_SIZE][MAX_SIZE], char copy[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            copy[row][col] = original[row][col];
        }
    }
}

void tilt_north(char dish[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int placement[MAX_SIZE] = {0};

    char dish_copy[MAX_SIZE][MAX_SIZE];
    copy_dish(dish, dish_copy, rows, cols);
    memset(dish, 0, MAX_SIZE * MAX_SIZE);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            switch (dish_copy[row][col]) {
                case 'O':
                    dish[placement[col]][col] = dish_copy[row][col];
                    placement[col]++;
                    break;
                case '#':
                    dish[row][col] = dish_copy[row][col];
                    placement[col] = row + 1;
                    break;
                default:
                    break;
            }
        }
    }
}

void tilt_west(char dish[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    char dish_copy[MAX_SIZE][MAX_SIZE];
    copy_dish(dish, dish_copy, rows, cols);
    memset(dish, 0, MAX_SIZE * MAX_SIZE);

    for (int row = 0; row < rows; row++) {
        int placement = 0;
        for (int col = 0; col < cols; col++) {
            switch (dish_copy[row][col]) {
                case 'O':
                    dish[row][placement] = dish_copy[row][col];
                    placement++;
                    break;
                case '#':
                    dish[row][col] = dish_copy[row][col];
                    placement = col + 1;
                    break;
                default:
                    break;
            }
        }
    }
}

void tilt_south(char dish[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int placement[MAX_SIZE] = {0};

    char dish_copy[MAX_SIZE][MAX_SIZE];
    copy_dish(dish, dish_copy, rows, cols);
    memset(dish, 0, MAX_SIZE * MAX_SIZE);

    for (int row = rows - 1; row >= 0; row--) {
        for (int col = 0; col < cols; col++) {
            switch (dish_copy[row][col]) {
                case 'O':
                    dish[rows - placement[col] - 1][col] = dish_copy[row][col];
                    placement[col]++;
                    break;
                case '#':
                    dish[row][col] = dish_copy[row][col];
                    placement[col] = rows - row;
                    break;
                default:
                    break;
            }
        }
    }
}

void tilt_east(char dish[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    char dish_copy[MAX_SIZE][MAX_SIZE];
    copy_dish(dish, dish_copy, rows, cols);
    memset(dish, 0, MAX_SIZE * MAX_SIZE);

    for (int row = 0; row < rows; row++) {
        int placement = cols - 1;
        for (int col = cols - 1; col >= 0; col--) {
            switch (dish_copy[row][col]) {
                case 'O':
                    dish[row][placement] = dish_copy[row][col];
                    placement--;
                    break;
                case '#':
                    dish[row][col] = dish_copy[row][col];
                    placement = col - 1;
                    break;
                default:
                    break;
            }
        }
    }
}

int calculate_load(char dish[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int load = 0;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (dish[row][col] == 'O') {
                load += rows - row;
            }
        }
    }
    return load;
}

bool is_repeat(int memory_cycles[MAX_CYCLES], int start, int length) {
    for (int i = start, j = start + length; i < start + length; i++, j++) {
        if (memory_cycles[i] != memory_cycles[j]) {
            return false;
        }
    }
    return true;
}

int solve_hard(char dish[MAX_SIZE][MAX_SIZE], int rows, int cols, int wanted_cycle) {
    int memory_loads[MAX_LOAD][2] = {0};
    int memory_cycles[MAX_CYCLES] = {0};

    for (int cycle = 1; cycle <= MAX_CYCLES; cycle++) {
        tilt_north(dish, rows, cols);
        tilt_west(dish, rows, cols);
        tilt_south(dish, rows, cols);
        tilt_east(dish, rows, cols);

        int load = calculate_load(dish, rows, cols);

        memory_cycles[cycle] = load;

        if (!memory_loads[load][0]) {
            memory_loads[load][0] = cycle;
        } else if (!memory_loads[load][1]) {
            memory_loads[load][1] = cycle;
        }
    }

    for (int cycle = 1; cycle < MAX_CYCLES; cycle++) {
        int load = memory_cycles[cycle];

        if (memory_loads[load][0] && memory_loads[load][1]) {
            int repeats_start = memory_loads[load][0];
            int repeats_length = memory_loads[load][1] - memory_loads[load][0];

            if (is_repeat(memory_cycles, repeats_start, repeats_length)) {
                int last_cycle = (repeats_start) + (wanted_cycle - repeats_start) % repeats_length;
                return memory_cycles[last_cycle];
            }
        }
    }

    return 0;
}

int solve(char dish[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    tilt_north(dish, rows, cols);
    return calculate_load(dish, rows, cols);
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int rows = 0, cols = 0;
    char dish[MAX_SIZE][MAX_SIZE] = {0};

    parse_dish(dish, &rows, &cols);

    int result = ishard ? solve_hard(dish, rows, cols, HARD_CYCLE) : solve(dish, rows, cols);
    printf("%d\n", result);
}
