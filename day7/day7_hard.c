#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define CARDS 13
#define HAND_SIZE 5
#define HANDS 1000

typedef enum card {
    JOKER,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    T,
    Q,
    K,
    A
} Card;

typedef enum hand_type {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
} HandType;

typedef struct hand {
    int bid;
    Card cards[HAND_SIZE];
    HandType hand_type;
} Hand;

Card get_card(char card) {
    switch (card) {
        case 'J':
            return JOKER;
        case '2':
            return TWO;
        case '3':
            return THREE;
        case '4':
            return FOUR;
        case '5':
            return FIVE;
        case '6':
            return SIX;
        case '7':
            return SEVEN;
        case '8':
            return EIGHT;
        case '9':
            return NINE;
        case 'T':
            return T;
        case 'Q':
            return Q;
        case 'K':
            return K;
        case 'A':
            return A;
    }

    return -1;
}

HandType get_hand_type(int card_counts[CARDS]) {
    bool has4of1 = false;
    bool has3of1 = false;
    int pairs = 0;

    int jokers = card_counts[0];
    for (int card_idx = 1; card_idx < CARDS; card_idx++) {
        switch (card_counts[card_idx]) {
            case 5:
                return FIVE_OF_A_KIND;
            case 4:
                has4of1 = true;
                break;
            case 3:
                has3of1 = true;
                break;
            case 2:
                pairs++;
                break;
        }
    }

    if ((has4of1 && jokers == 1) ||
        (has3of1 && jokers == 2) ||
        (pairs == 1 && jokers == 3) ||
        (jokers >= 4)) {
        return FIVE_OF_A_KIND;
    }

    if ((has4of1) ||
        (has3of1 && jokers == 1) ||
        (pairs == 1 && jokers == 2) ||
        (jokers == 3)) {
        return FOUR_OF_A_KIND;
    }

    if ((has3of1 && pairs == 1) ||
        (pairs == 2 && jokers == 1)) {
        return FULL_HOUSE;
    }

    if ((has3of1) ||
        (pairs == 1 && jokers == 1) ||
        (jokers == 2)) {
        return THREE_OF_A_KIND;
    }

    if (pairs == 2) {
        return TWO_PAIR;
    }

    if ((pairs == 1) || (jokers == 1)) {
        return ONE_PAIR;
    }

    return HIGH_CARD;
}

void parse_hands(Hand hands[HANDS], int *total_hands) {
    char hand_str[HAND_SIZE + 1] = {0};
    int bid;

    *total_hands = 0;
    while (scanf("%s %d\n", hand_str, &bid) > 0) {
        int card_counts[CARDS] = {0};
        for (int card_idx = 0; card_idx < HAND_SIZE; card_idx++) {
            Card curr_card = get_card(hand_str[card_idx]);
            hands[*total_hands].cards[card_idx] = curr_card;
            card_counts[curr_card]++;
        }
        hands[*total_hands].bid = bid;
        hands[*total_hands].hand_type = get_hand_type(card_counts);
        (*total_hands)++;
    }
}

int hand_comparator(const void *p0, const void *p1) {
    Hand *ph0 = (Hand *)p0;
    Hand *ph1 = (Hand *)p1;

    if (ph0->hand_type != ph1->hand_type) {
        return ph0->hand_type > ph1->hand_type;
    }

    for (int card_idx = 0; card_idx < HAND_SIZE; card_idx++) {
        if (ph0->cards[card_idx] != ph1->cards[card_idx]) {
            return ph0->cards[card_idx] > ph1->cards[card_idx];
        }
    }

    return 0;
}

int solve(Hand hands[HANDS], int total_hands) {
    int result = 0;

    qsort(hands, total_hands, sizeof(Hand), hand_comparator);

    for (int hand_idx = 0; hand_idx < total_hands; hand_idx++) {
        result += hands[hand_idx].bid * (hand_idx + 1);
    }

    return result;
}

int main() {
    int total_hands = 0;
    Hand hands[HANDS] = {0};
    parse_hands(hands, &total_hands);

    int result = solve(hands, total_hands);
    printf("%d\n", result);
}
