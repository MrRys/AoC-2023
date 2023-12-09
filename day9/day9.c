#include <stdbool.h>
#include <stdio.h>

#define MAX_HISTORY 23
#define MAX_LINE 200

void parse_history(char line[MAX_LINE], int history[MAX_HISTORY][MAX_HISTORY], int *history_len) {
    *history_len = 1;
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

        for (int c = r + 1; c < history_len; c++) {
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

int *solve() {
    char line[MAX_LINE] = {0};
    static int result[2] = {0};

    while (fgets(line, MAX_LINE, stdin)) {
        int history[MAX_HISTORY][MAX_HISTORY] = {0};
        int history_len = 0;
        int last_row = 0;

        parse_history(line, history, &history_len);

        generate_differences(history, history_len, &last_row);

        for (int r = last_row; r >= 0; r--) {
            history[r][history_len] = history[r][history_len - 1] + history[r + 1][history_len];
            history[r][r] = history[r][r + 1] - history[r + 1][r + 1];
        }

        result[0] += history[0][history_len];
        result[1] += history[0][0];
    }

    return result;
}

int main() {
    int *result = solve();
    printf("%d %d\n", result[0], result[1]);
}
