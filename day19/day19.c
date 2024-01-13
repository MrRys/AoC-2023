#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARTS 200
#define MAX_WORKFLOWS 252526
#define START_LABEL "in"

#define ACCEPT 'A' - 'a'
#define REJECT 'R' - 'a'

typedef enum category {
    NONE,
    X,
    M,
    A,
    S
} Category;

typedef struct part {
    Category categories[5];
} Part;

typedef struct condition {
    Category on_category;
    int compare_to;
    char operation;
    int dst;
} Condition;

typedef struct workflow {
    int condition_count;
    Condition conditions[10];
} Workflow;

Part parts[MAX_PARTS];
Workflow workflows[MAX_WORKFLOWS];

Category get_category(char category) {
    switch (tolower(category)) {
        case 'x':
            return X;
        case 'm':
            return M;
        case 'a':
            return A;
        case 's':
            return S;
        default:
            return NONE;
    }
}

int get_label_index(char *label) {
    int index = 0;
    for (int i = 0; label[i] != '\0'; i++) {
        index *= 100;
        index += label[i] - 'a';
    }
    return index;
}

void parse_input(Workflow *workflows, Part *parts, int *part_count) {
    char label[5] = {0};

    char line[50] = {0};

    while (fgets(line, 50, stdin) != NULL && line[0] != '\n') {
        int n, offset = 0;
        sscanf(line + offset, "%[^{]%*[{]%n", label, &n);
        offset += n;

        int workflow_idx = get_label_index(label);
        char cat, op;
        int opd;
        char dst_label[5] = {0};
        int cond = 0;

        while (sscanf(line + offset, "%1[xmas]%1[<>]%d:%[^,]%*[,]%n", &cat, &op, &opd, dst_label, &n) == 4) {
            workflows[workflow_idx].conditions[cond].on_category = get_category(cat);
            workflows[workflow_idx].conditions[cond].compare_to = opd;
            workflows[workflow_idx].conditions[cond].operation = op;
            workflows[workflow_idx].conditions[cond].dst = get_label_index(dst_label);
            cond++;
            offset += n;
        }

        sscanf(line + offset, "%[^}]%*[}]\n", dst_label);
        workflows[workflow_idx].conditions[cond].on_category = NONE;
        workflows[workflow_idx].conditions[cond].compare_to = 0;
        workflows[workflow_idx].conditions[cond].operation = 0;
        workflows[workflow_idx].conditions[cond].dst = get_label_index(dst_label);
        workflows[workflow_idx].condition_count = ++cond;
    }

    *part_count = 0;
    int x, m, a, s;
    while (scanf("{x=%d,m=%d,a=%d,s=%d}\n", &x, &m, &a, &s) > 0) {
        parts[*part_count].categories[X] = x;
        parts[*part_count].categories[M] = m;
        parts[*part_count].categories[A] = a;
        parts[*part_count].categories[S] = s;

        (*part_count)++;
    }
}

int accept(Part part) {
    int result = 0;
    for (int cat = X; cat <= S; cat++) {
        result += part.categories[cat];
    }
    return result;
}

int solve(Workflow *workflows, Part *parts, int parts_count) {
    int result = 0;

    int start_idx = get_label_index(START_LABEL);

    for (int part_idx = 0; part_idx < parts_count; part_idx++) {
        Workflow curr_workflow = workflows[start_idx];
        Part curr_part = parts[part_idx];
        bool finished = false;

        while (!finished) {
            for (int cond = 0; cond < curr_workflow.condition_count; cond++) {
                Category curr_cat = curr_workflow.conditions[cond].on_category;
                int curr_val = curr_part.categories[curr_cat];
                int curr_comp = curr_workflow.conditions[cond].compare_to;
                char curr_op = curr_workflow.conditions[cond].operation;
                int curr_dst = curr_workflow.conditions[cond].dst;

                if (curr_cat == NONE ||
                    (curr_op == '>' && curr_val > curr_comp) ||
                    (curr_op == '<' && curr_val < curr_comp)) {
                    switch (curr_dst) {
                        case ACCEPT:
                            result += accept(curr_part);
                            finished = true;
                            break;
                        case REJECT:
                            finished = true;
                            break;
                        default:
                            curr_workflow = workflows[curr_dst];
                    }

                    break;
                }
            }
        }
    }

    return result;
}

int main() {
    int parts_count = 0;
    parse_input(workflows, parts, &parts_count);

    int result = solve(workflows, parts, parts_count);
    printf("%d\n", result);
}
