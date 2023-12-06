#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RACES 10

void parse_races(int times[RACES], int distances[RACES], int *total_races) {
    scanf("%*s");
    *total_races = 0;
    while (scanf("%d", &times[*total_races]) > 0) {
        (*total_races)++;
    }

    scanf("%*s");
    *total_races = 0;
    while (scanf("%d", &distances[*total_races]) > 0) {
        (*total_races)++;
    }
}

void parse_single_race(long *time, long *distance) {
    scanf("%*s");
    char str_time[20] = {0};
    char c;
    int i = 0;
    while ((c = getc(stdin)) != '\n') {
        if (isdigit(c)) {
            str_time[i++] = c;
        }
    }
    *time = atol(str_time);

    scanf("%*s");
    char str_distance[20] = {0};
    i = 0;
    while ((c = getc(stdin)) != EOF) {
        if (isdigit(c)) {
            str_distance[i++] = c;
        }
    }
    *distance = atol(str_distance);
}

int solve(int times[RACES], int distances[RACES], int total_races) {
    int result = 1;

    for (int race_idx = 0; race_idx < total_races; race_idx++) {
        int curr_time = times[race_idx];
        int curr_dist = distances[race_idx];
        int count = 0;

        for (int hold = 1; hold < curr_time - 1; hold++) {
            if (hold * (curr_time - hold) > curr_dist) {
                count++;
            }
        }

        result *= count;
    }

    return result;
}

int solve_hard(long time, long distance) {
    int result = 0;
    for (long hold = 1; hold < time - 1; hold++) {
        if (hold * (time - hold) > distance) {
            result++;
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    long time = 0, distance = 0;
    int total_races = 0;
    int times[RACES] = {0};
    int distances[RACES] = {0};

    ishard ? parse_single_race(&time, &distance) : parse_races(times, distances, &total_races);

    int result = ishard ? solve_hard(time, distance) : solve(times, distances, total_races);
    printf("%d\n", result);
}
