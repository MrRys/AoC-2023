#include <stdbool.h>
#include <stdio.h>

#define LIMIT_RED 12
#define LIMIT_GREEN 13
#define LIMIT_BLUE 14

#define MAX(a, b) (a > b ? a : b)

bool check_possible(char color, int count) {
    switch (color) {
        case 'r':
            return count <= LIMIT_RED;
        case 'g':
            return count <= LIMIT_GREEN;
        case 'b':
            return count <= LIMIT_BLUE;
        default:
            return false;
    }
}

int solve() {
    int result = 0;

    char color;
    int game, count;
    while (scanf("Game %d:", &game) > 0) {
        bool ispossible = true;
        while (scanf("%d %c%*[^ \n]", &count, &color) > 0) {
            if (!check_possible(color, count)) {
                ispossible = false;
            }
        }

        if (ispossible) {
            result += game;
        }
    }
    return result;
}

int solve_hard() {
    int result = 0;

    char color;
    int game, count;
    while (scanf("Game %d:", &game) > 0) {
        int r = 0, g = 0, b = 0;
        while (scanf("%d %c%*[^ \n]", &count, &color) > 0) {
            switch (color) {
                case 'r':
                    r = MAX(r, count);
                    break;
                case 'g':
                    g = MAX(g, count);
                    break;
                case 'b':
                    b = MAX(b, count);
                    break;
                default:
                    break;
            }
        }
        result += r * g * b;
    }
    return result;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int result = ishard ? solve_hard() : solve();
    printf("%d\n", result);
}