#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

#define SEEDS 150
#define MAPS 7
#define RANGES 50

void parse_input(long seeds[SEEDS], long seed_ranges[SEEDS][2], long maps[MAPS][RANGES][3], int *total_seeds, int *total_seed_ranges) {
    scanf("%*s");

    *total_seeds = 0;
    while (scanf("%ld", &seeds[*total_seeds]) > 0) {
        if (*total_seeds % 2 == 0) {
            seed_ranges[*total_seeds / 2][0] = seeds[*total_seeds];
        } else {
            seed_ranges[*total_seeds / 2][1] = seed_ranges[*total_seeds / 2][0] + seeds[*total_seeds] - 1;
        }

        (*total_seeds)++;
    }

    *total_seed_ranges = *total_seeds / 2;

    int map_idx = 0;
    char line[30];
    while (scanf("%[^\n]\n", line) > 0) {
        int range_idx = 0;
        long dst, src, range;
        while (scanf("%ld %ld %ld", &dst, &src, &range) > 0) {
            maps[map_idx][range_idx][0] = src;
            maps[map_idx][range_idx][1] = src + range - 1;
            maps[map_idx][range_idx][2] = dst - src;
            range_idx++;
        }
        map_idx++;
    }
}

bool is_valid_range(long range[3]) {
    return range[0] || range[1] || range[2];
}

long find_min_seed(long *array, int length) {
    long min = LONG_MAX;
    for (int i = 0; i < length; i++) {
        min = MIN(min, array[i]);
    }
    return min;
}

long solve(long seeds[SEEDS], long maps[MAPS][RANGES][3], int total_seeds) {
    for (int seed_idx = 0; seed_idx < total_seeds; seed_idx++) {
        for (int map_idx = 0; map_idx < MAPS; map_idx++) {
            for (int range_idx = 0; is_valid_range(maps[map_idx][range_idx]); range_idx++) {
                long seed = seeds[seed_idx];
                long range_from = maps[map_idx][range_idx][0];
                long range_to = maps[map_idx][range_idx][1];
                long conversion = maps[map_idx][range_idx][2];

                if (seed >= range_from && seed <= range_to) {
                    seeds[seed_idx] += conversion;
                    break;
                }
            }
        }
    }

    return find_min_seed(seeds, total_seeds);
}

long find_min_seed_range(long (*array)[2], int length) {
    long min = LONG_MAX;
    for (int i = 0; i < length; i++) {
        min = MIN(min, array[i][0]);
    }

    return min;
}

long solve_hard(long old_seed_ranges[SEEDS][2], long maps[MAPS][RANGES][3], int old_total) {
    long new_seed_ranges[SEEDS][2] = {0};
    long(*handle_old)[2] = old_seed_ranges;
    long(*handle_new)[2] = new_seed_ranges;

    for (int map_idx = 0; map_idx < MAPS; map_idx++) {
        int new_total = 0;

        for (int seed_idx = 0; seed_idx < old_total; seed_idx++) {
            long seed_from = handle_old[seed_idx][0];
            long seed_to = handle_old[seed_idx][1];

            for (int range_idx = 0; is_valid_range(maps[map_idx][range_idx]); range_idx++) {
                long range_from = maps[map_idx][range_idx][0];
                long range_to = maps[map_idx][range_idx][1];
                long conversion = maps[map_idx][range_idx][2];

                long start = MAX(seed_from, range_from);
                long end = MIN(seed_to, range_to);

                if (start > end) {
                    if (!is_valid_range(maps[map_idx][range_idx + 1])) {
                        handle_new[new_total][0] = seed_from;
                        handle_new[new_total][1] = seed_to;
                        new_total++;
                    }
                    continue;
                }

                handle_new[new_total][0] = start + conversion;
                handle_new[new_total][1] = end + conversion;
                new_total++;

                if (start > seed_from) {
                    handle_old[old_total][0] = seed_from;
                    handle_old[old_total][1] = start - 1;
                    old_total++;
                }

                if (end < seed_to) {
                    handle_old[old_total][0] = end + 1;
                    handle_old[old_total][1] = seed_to;
                    old_total++;
                }

                break;
            }
        }

        long(*handle_tmp)[2] = handle_old;
        handle_old = handle_new;
        handle_new = handle_tmp;
        old_total = new_total;
    }

    return find_min_seed_range(handle_old, old_total);
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int total_seeds = 0;
    int total_seed_ranges = 0;
    long seeds[SEEDS] = {0};
    long seed_ranges[SEEDS][2] = {0};
    long maps[MAPS][RANGES][3] = {0};

    parse_input(seeds, seed_ranges, maps, &total_seeds, &total_seed_ranges);

    long result = ishard ? solve_hard(seed_ranges, maps, total_seed_ranges) : solve(seeds, maps, total_seeds);
    printf("%ld\n", result);
}