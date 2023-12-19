#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define TOTAL_RECORDS 1000
#define RECORD_LENGTH 200
#define GROUPS_LENGTH 100
#define EASY_MULTIPLIER 1
#define HARD_MULTIPLIER 5
#define MAX_MEMORY 1500
#define NOT_FOUND (unsigned long)(-1)

void parse_records(char records_as_str[TOTAL_RECORDS][RECORD_LENGTH], int records_as_dmg_groups[TOTAL_RECORDS][GROUPS_LENGTH], int *record_count, int multiplier) {
    *record_count = 0;

    while (scanf("%s", records_as_str[*record_count]) > 0) {
        int group_idx = 0;

        while (scanf("%d,", &records_as_dmg_groups[*record_count][group_idx + 1]) > 0) {
            group_idx++;
        }

        int record_length = strlen(records_as_str[*record_count]);

        for (int i = 1; i < multiplier; i++) {
            records_as_str[*record_count][record_length * i + (i - 1)] = '?';
            for (int c = 0; c < record_length; c++) {
                records_as_str[*record_count][(record_length + 1) * i + c] = records_as_str[*record_count][c];
            }
            for (int c = 1; c < group_idx + 1; c++) {
                records_as_dmg_groups[*record_count][group_idx * i + c] = records_as_dmg_groups[*record_count][c];
            }
        }

        records_as_dmg_groups[*record_count][0] = group_idx * multiplier + 1;
        (*record_count)++;
    }
}

bool is_valid(char record[RECORD_LENGTH], int size) {
    int record_len = strlen(record);
    if (record_len < size) {
        return false;
    }

    for (int i = 0; i < size; i++) {
        if (record[i] == '.') {
            return false;
        }
    }

    return record_len == size || record[size] != '#';
}

bool contains(char c, char *string) {
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == c) {
            return true;
        }
    }
    return false;
}

unsigned long hash_str(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

unsigned long hash_int(int *arr, int len) {
    unsigned long hash = 0;
    for (int i = 0; i < len; i++) {
        if (arr[i] < 10) {
            hash *= 10;
        } else {
            hash *= 100;
        }
        hash += arr[i];
    }
    return hash;
}

unsigned long find(unsigned long memory[MAX_MEMORY][3], unsigned long hash_s, unsigned long hash_i) {
    for (int i = 1; i <= (int)memory[0][0]; i++) {
        if (memory[i][0] == hash_s && memory[i][1] == hash_i) {
            return memory[i][2];
        }
    }
    return NOT_FOUND;
}

unsigned long find_possible(unsigned long memory[MAX_MEMORY][3], char record[RECORD_LENGTH], int dmg_groups[GROUPS_LENGTH], int record_length, int groups, int record_idx, int group_idx) {
    if (record_idx >= record_length) {
        return group_idx == groups;
    }

    if (group_idx >= groups) {
        return !contains('#', record + record_idx);
    }

    unsigned long hash_s = hash_str(record + record_idx);
    unsigned long hash_i = hash_int(dmg_groups + group_idx, groups - group_idx + 1);
    unsigned long memorized = find(memory, hash_s, hash_i);

    if (memorized != NOT_FOUND) {
        return memorized;
    }

    unsigned long result = 0;

    if (record[record_idx] == '.' || record[record_idx] == '?') {
        result += find_possible(memory, record, dmg_groups, record_length, groups, record_idx + 1, group_idx);
    }

    if ((record[record_idx] == '#' || record[record_idx] == '?') && is_valid(record + record_idx, dmg_groups[group_idx])) {
        result += find_possible(memory, record, dmg_groups, record_length, groups, record_idx + dmg_groups[group_idx] + 1, group_idx + 1);
    }

    int idx = ++memory[0][0];
    memory[idx][0] = hash_s;
    memory[idx][1] = hash_i;
    memory[idx][2] = result;

    return result;
}

unsigned long solve(char records_as_str[TOTAL_RECORDS][RECORD_LENGTH], int records_as_dmg_groups[TOTAL_RECORDS][GROUPS_LENGTH], int record_count) {
    unsigned long result = 0;

    for (int record_idx = 0; record_idx < record_count; record_idx++) {
        unsigned long memory[MAX_MEMORY][3] = {0};
        result += find_possible(memory, records_as_str[record_idx], records_as_dmg_groups[record_idx], strlen(records_as_str[record_idx]), records_as_dmg_groups[record_idx][0], 0, 1);
    }

    return result;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int record_count = 0;
    char records_as_str[TOTAL_RECORDS][RECORD_LENGTH] = {0};
    int records_as_dmg_groups[TOTAL_RECORDS][GROUPS_LENGTH] = {0};

    int multiplier = ishard ? HARD_MULTIPLIER : EASY_MULTIPLIER;
    parse_records(records_as_str, records_as_dmg_groups, &record_count, multiplier);

    unsigned long result = solve(records_as_str, records_as_dmg_groups, record_count);
    printf("%lu\n", result);
}
