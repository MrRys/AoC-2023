#include <stdbool.h>
#include <stdio.h>

#define MAX_HISTORY 21
#define MAX_LINE 200

void parse_history(char line[MAX_LINE], int history[MAX_HISTORY][MAX_HISTORY], int *history_len) {
    *history_len = 0;

    int offset = 0;
    int read;

    while (sscanf(line + offset, "%d%n", &history[0][*history_len], &read) > 0) {
        (*history_len)++;
        offset += read;
    }
}

void generate_differences(int history[MAX_HISTORY][MAX_HISTORY], int history_len, int *last_row) {
    for (int r = 1; r < history_len; r++) {
        bool all_zero = true;

        for (int c = r; c < history_len; c++) {
            history[r][c] = history[r - 1][c] - history[r - 1][c - 1];

            if (history[r][c] != 0) {
                all_zero = false;
            }
        }

        if (all_zero) {
            return;
        }

        *last_row = r;
    }
}

int solve_rec(int arr[MAX_HISTORY], int len, bool all_zero, bool ishard) {
    if (all_zero) {
        return 0;
    }

    int diff[MAX_HISTORY] = {0};

    int i = 0;
    all_zero = true;
    for (i = 0; i < len - 1; i++) {
        diff[i] = arr[i + 1] - arr[i];
        if (diff[i] != 0) {
            all_zero = false;
        }
    }

    if (ishard) {
        return arr[0] - solve_rec(diff, len - 1, all_zero, ishard);
    } else {
        return arr[len - 1] + solve_rec(diff, len - 1, all_zero, ishard);
    }
}

int solve(int history[MAX_HISTORY][MAX_HISTORY], int history_len, bool ishard) {
    int result = 0;

    int last_row = 0;
    generate_differences(history, history_len, &last_row);

    if (ishard) {
        for (int r = last_row; r >= 0; r--) {
            result = history[r][r] - result;
        }
    } else {
        for (int r = last_row; r >= 0; r--) {
            result = history[r][history_len - 1] + result;
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && (argv[1][0] == 'h' || argv[1][1] == 'h');
    bool isrec = argc > 1 && (argv[1][0] == 'r' || argv[1][1] == 'r');

    char line[MAX_LINE] = {0};
    int result = 0;

    while (fgets(line, MAX_LINE, stdin)) {
        int history[MAX_HISTORY][MAX_HISTORY] = {0};
        int history_len = 0;

        parse_history(line, history, &history_len);

        if (isrec) {
            result += solve_rec(history[0], history_len, false, ishard);
        } else {
            result += solve(history, history_len, ishard);
        }
    }

    printf("%d\n", result);
}
