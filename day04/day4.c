#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_CARDS 200
#define MAX_NUMBERS 25
#define MAX_LINE 150

void parse_cards(int cards[MAX_CARDS][2][MAX_NUMBERS], int *total_cards) {
    int card;
    char line[MAX_LINE];
    while (scanf("Card %d:", &card) > 0) {
        (*total_cards)++;
        fgets(line, MAX_LINE, stdin);

        char *token = strtok(line, "|");
        token = strtok(NULL, "|");

        int i = 0;
        int total = 0;
        int scanned;
        while (sscanf(line + total, "%d%n", &cards[card - 1][0][i], &scanned) > 0) {
            i++;
            total += scanned;
        }
        i = 0;
        total = 0;
        while (sscanf(token + total, "%d%n", &cards[card - 1][1][i], &scanned) > 0) {
            i++;
            total += scanned;
        }
    }
}

int solve(int cards[MAX_CARDS][2][MAX_NUMBERS], int total_cards) {
    int result = 0;
    for (int card = 0; card < total_cards; card++) {
        int have = 0;
        bool winning_numbers[100] = {false};
        for (int n = 0; n < MAX_NUMBERS; n++) {
            int number = cards[card][0][n];
            winning_numbers[number] = true;
        }

        for (int n = 0; n < MAX_NUMBERS; n++) {
            int number = cards[card][1][n];
            if (number > 0 && winning_numbers[number]) {
                have++;
            }
        }

        result += (int)pow((double)2, (double)(have - 1));
    }
    return result;
}

int solve_hard(int cards[MAX_CARDS][2][MAX_NUMBERS], int total_cards) {
    int result = 0;
    int owned_cards[MAX_CARDS] = {0};
    for (int card = 0; card < total_cards; card++) {
        owned_cards[card] += 1;

        int won = 0;
        bool winning_numbers[100] = {false};

        for (int n = 0; n < MAX_NUMBERS; n++) {
            int number = cards[card][0][n];
            winning_numbers[number] = true;
        }

        for (int n = 0; n < MAX_NUMBERS; n++) {
            int number = cards[card][1][n];
            if (number > 0 && winning_numbers[number]) {
                won++;
            }
        }

        for (int c = card + 1; won > 0; c++, won--) {
            owned_cards[c] += owned_cards[card];
        }
    }

    for (int card = 0; card < MAX_CARDS; card++) {
        result += owned_cards[card];
    }

    return result;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int total_cards = 0;
    int cards[MAX_CARDS][2][MAX_NUMBERS] = {0};
    parse_cards(cards, &total_cards);

    int result = ishard ? solve_hard(cards, total_cards) : solve(cards, total_cards);
    printf("%d\n", result);
}