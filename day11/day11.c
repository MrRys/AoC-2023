#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAP_SIZE 141
#define UNIVERSE_SIZE 500
#define EASY_EXPANSION 2
#define HARD_EXPANSION 1000000

void parse_universe(int galaxies[UNIVERSE_SIZE][2], bool universe_rows[UNIVERSE_SIZE], bool universe_cols[UNIVERSE_SIZE], int *galaxy_count) {
    *galaxy_count = 0;
    for (int row = 0; row < UNIVERSE_SIZE; row++) {
        for (int col = 0; col < UNIVERSE_SIZE; col++) {
            char c = fgetc(stdin);
            if (c == '\n') {
                break;
            }

            if (c == '#') {
                universe_rows[row] = true;
                universe_cols[col] = true;

                galaxies[*galaxy_count][0] = row;
                galaxies[*galaxy_count][1] = col;
                (*galaxy_count)++;
            }
        }
    }
}

int manhattan_distance(int r1, int c1, int r2, int c2) {
    return (abs(r1 - r2) + abs(c1 - c2));
}

int expansion(bool universe_rows[UNIVERSE_SIZE], bool universe_cols[UNIVERSE_SIZE], int r1, int c1, int r2, int c2) {
    int count = 0;

    int min = r1 < r2 ? r1 : r2;
    int max = r1 > r2 ? r1 : r2;

    for (int r = min; r <= max; r++) {
        if (!universe_rows[r]) {
            count++;
        }
    }

    min = c1 < c2 ? c1 : c2;
    max = c1 > c2 ? c1 : c2;

    for (int c = min; c <= max; c++) {
        if (!universe_cols[c]) {
            count++;
        }
    }

    return count;
}

long solve(int galaxies[UNIVERSE_SIZE][2], bool universe_rows[UNIVERSE_SIZE], bool universe_cols[UNIVERSE_SIZE], int galaxy_count, int expansion_rate) {
    long result = 0;

    for (int g1 = 0; g1 < galaxy_count - 1; g1++) {
        for (int g2 = g1; g2 < galaxy_count; g2++) {
            if (g1 == g2) {
                continue;
            }
            int r1 = galaxies[g1][0], c1 = galaxies[g1][1], r2 = galaxies[g2][0], c2 = galaxies[g2][1];
            result += manhattan_distance(r1, c1, r2, c2) + expansion(universe_rows, universe_cols, r1, c1, r2, c2) * (expansion_rate - 1);
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int galaxy_count = 0;
    int galaxies[UNIVERSE_SIZE][2] = {0};
    bool universe_rows[UNIVERSE_SIZE] = {0};
    bool universe_cols[UNIVERSE_SIZE] = {0};

    parse_universe(galaxies, universe_rows, universe_cols, &galaxy_count);

    int expansion_rate = ishard ? HARD_EXPANSION : EASY_EXPANSION;
    long result = solve(galaxies, universe_rows, universe_cols, galaxy_count, expansion_rate);
    printf("%ld\n", result);
}
