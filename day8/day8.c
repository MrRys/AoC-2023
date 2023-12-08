#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ulong unsigned long

#define INST_LEN 300
#define START_IDXS_LEN 2526
#define NODE_LEN 252526

#define AAA_NODE 0
#define ZZZ_NODE 252525

int parse_int(char *str) {
    int num = 0;
    int multiplier = 1;
    for (char *c = str; *c != '\0'; c++) {
        num += (*c - 'A') * multiplier;
        multiplier *= 100;
    }
    return num;
}

void parse_input(int nodes[NODE_LEN][2], char instructions[INST_LEN], int start_idxs[START_IDXS_LEN], int *idxs_len) {
    scanf("%s", instructions);

    char from_str[4] = {0}, left_str[4] = {0}, right_str[4] = {0};

    int start_idx = 0;

    while (scanf("\n%3c = (%3c, %3c)\n", from_str, left_str, right_str) > 0) {
        int from = parse_int(from_str);
        int left = parse_int(left_str);
        int right = parse_int(right_str);

        nodes[from][0] = left;
        nodes[from][1] = right;

        if (from < START_IDXS_LEN) {
            start_idxs[start_idx++] = from;
        }
    }

    *idxs_len = start_idx;
}

ulong solve(int nodes[NODE_LEN][2], char instructions[INST_LEN], int start_idx, int end_index) {
    int inst_len = strlen(instructions);
    int instruction = 0;

    ulong steps = 0;
    int curr_node = start_idx;
    while (curr_node < end_index) {
        switch (instructions[instruction++ % inst_len]) {
            case 'L':
                curr_node = nodes[curr_node][0];
                break;
            case 'R':
                curr_node = nodes[curr_node][1];
                break;
        }
        steps++;
    }

    return steps;
}

ulong gcd(ulong a, ulong b) {
    if (b == 0) {
        return a;
    }

    return gcd(b, a % b);
}

ulong lcm(ulong arr[], int n) {
    ulong ans = arr[0];

    for (int i = 1; i < n; i++) {
        ans = arr[i] * ans / gcd(arr[i], ans);
    }

    return ans;
}

ulong solve_hard(int nodes[NODE_LEN][2], char instructions[INST_LEN], int start_idxs[START_IDXS_LEN], int idxs_len) {
    ulong all_steps[idxs_len];

    for (int idx = 0; idx < idxs_len; idx++) {
        all_steps[idx] = solve(nodes, instructions, start_idxs[idx], 250000);
    }

    return lcm(all_steps, idxs_len);
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    char instructions[INST_LEN] = {0};
    int nodes[NODE_LEN][2] = {0};

    int idxs_len = 0;
    int start_idxs[START_IDXS_LEN];

    parse_input(nodes, instructions, start_idxs, &idxs_len);

    ulong result = ishard ? solve_hard(nodes, instructions, start_idxs, idxs_len) : solve(nodes, instructions, AAA_NODE, ZZZ_NODE);
    printf("%lu\n", result);
}
