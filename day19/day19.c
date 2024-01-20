#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PARTS 200
#define MAX_WORKFLOWS 252526
#define START_LABEL "in"

#define MIN_RANGE 1
#define MAX_RANGE 4000

#define ACCEPT_IDX 'A' - 'a'
#define REJECT_IDX 'R' - 'a'

#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b

#define lu unsigned long

typedef enum category {
    NONE,
    X,
    M,
    A,
    S
} Category;

typedef struct part {
    Category ctgs[5];
} Part;

typedef struct condition {
    Category on_ctg;
    char cmp_op;
    int cmp_to;
    int dst_idx;
} Condition;

typedef struct workflow {
    int cnd_count;
    Condition cnds[5];
} Workflow;

Category get_category(char ctg) {
    switch (tolower(ctg)) {
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

int get_label_index(const char *label) {
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
        int read_c, offset = 0;
        sscanf(line, "%[^{]%*[{]%n", label, &read_c);
        offset += read_c;

        int workflow_idx = get_label_index(label);
        char ctg, cmp_op;
        int cmp_to;
        char dst_label[5] = {0};
        int cnd_idx = 0;

        while (sscanf(line + offset, "%1[xmas]%1[<>]%d:%[^,]%*[,]%n", &ctg, &cmp_op, &cmp_to, dst_label, &read_c) == 4) {
            workflows[workflow_idx].cnds[cnd_idx].on_ctg = get_category(ctg);
            workflows[workflow_idx].cnds[cnd_idx].cmp_op = cmp_op;
            workflows[workflow_idx].cnds[cnd_idx].cmp_to = cmp_to;
            workflows[workflow_idx].cnds[cnd_idx].dst_idx = get_label_index(dst_label);
            cnd_idx++;
            offset += read_c;
        }

        sscanf(line + offset, "%[^}]%*[}]\n", dst_label);
        workflows[workflow_idx].cnds[cnd_idx].on_ctg = NONE;
        workflows[workflow_idx].cnds[cnd_idx].cmp_op = 0;
        workflows[workflow_idx].cnds[cnd_idx].cmp_to = 0;
        workflows[workflow_idx].cnds[cnd_idx].dst_idx = get_label_index(dst_label);
        workflows[workflow_idx].cnd_count = ++cnd_idx;
    }

    *part_count = 0;
    int x, m, a, s;
    while (scanf("{x=%d,m=%d,a=%d,s=%d}\n", &x, &m, &a, &s) > 0) {
        parts[*part_count].ctgs[X] = x;
        parts[*part_count].ctgs[M] = m;
        parts[*part_count].ctgs[A] = a;
        parts[*part_count].ctgs[S] = s;

        (*part_count)++;
    }
}

lu accept_single(Part part) {
    lu result = 0;
    for (int ctg = X; ctg <= S; ctg++) {
        result += part.ctgs[ctg];
    }
    return result;
}

lu solve(const Workflow *workflows, const Part *parts, int parts_count) {
    lu result = 0;

    int start_idx = get_label_index(START_LABEL);

    for (int part_idx = 0; part_idx < parts_count; part_idx++) {
        Workflow curr_workflow = workflows[start_idx];
        Part curr_part = parts[part_idx];
        bool finished = false;

        while (!finished) {
            for (int cnd_idx = 0; cnd_idx < curr_workflow.cnd_count; cnd_idx++) {
                Condition curr_cnd = curr_workflow.cnds[cnd_idx];
                int curr_val = curr_part.ctgs[curr_cnd.on_ctg];

                if (curr_cnd.cmp_op == 0 ||
                    (curr_cnd.cmp_op == '>' && curr_val > curr_cnd.cmp_to) ||
                    (curr_cnd.cmp_op == '<' && curr_val < curr_cnd.cmp_to)) {
                    switch (curr_cnd.dst_idx) {
                        case ACCEPT_IDX:
                            result += accept_single(curr_part);
                            finished = true;
                            break;
                        case REJECT_IDX:
                            finished = true;
                            break;
                        default:
                            curr_workflow = workflows[curr_cnd.dst_idx];
                    }

                    break;
                }
            }
        }
    }

    return result;
}

void init_ranges(int ranges[5][2]) {
    for (int i = X; i <= S; i++) {
        ranges[i][0] = MIN_RANGE;
        ranges[i][1] = MAX_RANGE;
    }
}

void copy_ranges(const int old_ranges[5][2], int new_ranges[5][2]) {
    for (int i = X; i <= S; i++) {
        new_ranges[i][0] = old_ranges[i][0];
        new_ranges[i][1] = old_ranges[i][1];
    }
}

lu accept_ranges(int ranges[5][2]) {
    lu result = 1;
    for (int i = X; i <= S; i++) {
        result *= ranges[i][1] - ranges[i][0] + 1;
    }
    return result;
}

lu count_accepted(Workflow *workflows, int src_idx, int ranges[5][2]) {
    if (src_idx == ACCEPT_IDX) {
        return accept_ranges(ranges);
    } else if (src_idx == REJECT_IDX) {
        return 0;
    }

    Workflow curr_workflow = workflows[src_idx];
    lu result = 0;

    for (int cnd_idx = 0; cnd_idx < curr_workflow.cnd_count; cnd_idx++) {
        Condition curr_cnd = curr_workflow.cnds[cnd_idx];
        int min_r = ranges[curr_cnd.on_ctg][0];
        int max_r = ranges[curr_cnd.on_ctg][1];

        int acc_range[2] = {0};
        int rej_range[2] = {0};

        if (curr_cnd.cmp_op == '>') {
            acc_range[0] = MAX(curr_cnd.cmp_to + 1, min_r);
            acc_range[1] = max_r;
            rej_range[0] = min_r;
            rej_range[1] = MIN(curr_cnd.cmp_to, max_r);
        } else if (curr_cnd.cmp_op == '<') {
            acc_range[0] = min_r;
            acc_range[1] = MIN(curr_cnd.cmp_to - 1, max_r);
            rej_range[0] = MAX(curr_cnd.cmp_to, min_r);
            rej_range[1] = max_r;
        } else {
            result += count_accepted(workflows, curr_cnd.dst_idx, ranges);
            break;
        }

        // count ranges that would move to a new workflow
        int new_ranges[5][2] = {0};
        copy_ranges(ranges, new_ranges);
        new_ranges[curr_cnd.on_ctg][0] = acc_range[0];
        new_ranges[curr_cnd.on_ctg][1] = acc_range[1];
        result += count_accepted(workflows, curr_cnd.dst_idx, new_ranges);

        // cut ranges on the current workflow
        ranges[curr_cnd.on_ctg][0] = rej_range[0];
        ranges[curr_cnd.on_ctg][1] = rej_range[1];
    }

    return result;
}

lu solve_hard(Workflow *worklfows) {
    int ranges[5][2] = {0};
    init_ranges(ranges);
    int start_idx = get_label_index(START_LABEL);

    return count_accepted(worklfows, start_idx, ranges);
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    Part *parts = (Part *)calloc(MAX_PARTS, sizeof(Part));
    Workflow *workflows = (Workflow *)calloc(MAX_WORKFLOWS, sizeof(Workflow));

    int parts_count = 0;
    parse_input(workflows, parts, &parts_count);

    lu result = ishard ? solve_hard(workflows) : solve(workflows, parts, parts_count);
    printf("%lu\n", result);

    free(parts);
    free(workflows);
}
