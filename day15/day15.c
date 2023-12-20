#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LABEL_SIZE 15
#define MAP_SIZE 256

typedef struct item {
    char label[LABEL_SIZE];
    int lens;
    struct item *next;
} Item;

Item *init_item(char label[LABEL_SIZE], int lens) {
    Item *item = (Item *)malloc(sizeof(Item));
    strncpy(item->label, label, LABEL_SIZE);
    item->lens = lens;
    item->next = NULL;
    return item;
}

void free_map(Item *map[MAP_SIZE]) {
    for (int box = 0; box < MAP_SIZE; box++) {
        if (map[box] == NULL) {
            continue;
        }

        Item *curr = map[box];
        while (curr != NULL) {
            Item *prev = curr;
            curr = curr->next;
            free(prev);
        }
    }
}

int hash_label(char *label) {
    int hash = 0;
    for (int i = 0; label[i] != '\0'; i++) {
        hash += label[i];
        hash *= 17;
        hash %= MAP_SIZE;
    }
    return hash;
}

int calculate_focusing_power(Item *map[MAP_SIZE]) {
    int focusing_power = 0;

    for (int box = 0; box < MAP_SIZE; box++) {
        if (map[box] == NULL) {
            continue;
        }

        int slot = 1;
        Item *curr = map[box];
        while (curr != NULL) {
            focusing_power += (box + 1) * slot * curr->lens;
            curr = curr->next;
            slot++;
        }
    }

    return focusing_power;
}

void add_item(Item *map[MAP_SIZE], char label[LABEL_SIZE], int lens) {
    int hash = hash_label(label);

    if (map[hash] == NULL) {
        map[hash] = init_item(label, lens);
        return;
    }

    Item *curr = map[hash];
    while (true) {
        if (strncmp(curr->label, label, LABEL_SIZE) == 0) {
            curr->lens = lens;
            return;
        }

        if (curr->next == NULL) {
            break;
        }

        curr = curr->next;
    }

    curr->next = init_item(label, lens);
}

void remove_item(Item *map[MAP_SIZE], char label[LABEL_SIZE]) {
    int hash = hash_label(label);

    Item *curr = map[hash];
    Item *prev = NULL;

    while (curr != NULL) {
        if (strncmp(curr->label, label, LABEL_SIZE) == 0) {
            if (curr == map[hash]) {
                map[hash] = curr->next;
            } else {
                prev->next = curr->next;
            }

            free(curr);
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}

int solve_hard() {
    Item *map[MAP_SIZE] = {0};

    char step[LABEL_SIZE] = {0};
    while (scanf("%[^,]%*c", step) > 0) {
        int len = strlen(step);

        if (step[len - 1] == '-') {
            step[len - 1] = '\0';
            remove_item(map, step);
        }

        if (step[len - 2] == '=') {
            int lens = step[len - 1] - '0';
            step[len - 2] = '\0';
            add_item(map, step, lens);
        }
    }

    int focusing_power = calculate_focusing_power(map);

    free_map(map);

    return focusing_power;
}

int solve() {
    int result = 0;

    char step[LABEL_SIZE] = {0};
    while (scanf("%[^,]%*c", step) > 0) {
        result += hash_label(step);
    }

    return result;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';
    int result = ishard ? solve_hard() : solve();
    printf("%d\n", result);
}
