#define _POSIX_C_SOURCE 200809L

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_SIZE 150
#define QUEUE_SIZE (MAP_SIZE * MAP_SIZE)

int SAME_DIR_MINIMUM;
int SAME_DIR_MAXIMUM;

int START_ROW;
int START_COL;
int GOAL_ROW;
int GOAL_COL;

typedef enum direction {
    NO_DIR,
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct node {
    Direction dir;
    int same_dir_count;
    int row;
    int col;
    int g_score;
    int f_score;
    struct node *parent;
} Node;

void parse_map(char map[MAP_SIZE][MAP_SIZE], int *height, int *width) {
    *width = *height = 0;

    // tried to be super safe lol
    char format[20] = {0};
    snprintf(format, 19, "%%%d[^\n]%%*c", MAP_SIZE - 1);

    while (scanf(format, map[*height]) > 0) {
        (*height)++;
    }

    *width = strnlen(map[0], MAP_SIZE);
}

Node *init_node(Direction dir, int same_dir_count, int row, int col, int g_score, int f_score, Node *parent) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->dir = dir;
    new_node->same_dir_count = same_dir_count;
    new_node->row = row;
    new_node->col = col;
    new_node->g_score = g_score;
    new_node->f_score = f_score;
    new_node->parent = parent;
    return new_node;
}

int manhattan_distance(int r1, int c1, int r2, int c2) {
    return abs((r1 - r2) + (c1 - c2));
}

void push(Node *pqueue[QUEUE_SIZE], int *qsize, Node *node) {
    pqueue[*qsize] = node;
    (*qsize)++;
}

Node *pop(Node *pqueue[QUEUE_SIZE], int *qsize) {
    Node *min = pqueue[0];
    int min_i = 0;

    for (int i = 1; i < *qsize; i++) {
        if (min->g_score >= pqueue[i]->g_score) {
            min = pqueue[i];
            min_i = i;
        }
    }

    for (int i = min_i + 1; i <= *qsize; i++) {
        pqueue[i - 1] = pqueue[i];
    }

    (*qsize)--;

    return min;
}

int get_cost(char block) {
    return block - '0';
}

bool move(int height, int width, Node *curr_node, Direction new_dir, int *row, int *col) {
    if (curr_node->dir == new_dir && curr_node->same_dir_count >= SAME_DIR_MAXIMUM) {
        return false;
    }

    *row = curr_node->row;
    *col = curr_node->col;

    switch (new_dir) {
        case UP:
            if (curr_node->dir != DOWN && curr_node->row - 1 >= 0) {
                (*row)--;
                return true;
            }
            break;
        case DOWN:
            if (curr_node->dir != UP && curr_node->row + 1 < height) {
                (*row)++;
                return true;
            }
            break;
        case LEFT:
            if (curr_node->dir != RIGHT && curr_node->col - 1 >= 0) {
                (*col)--;
                return true;
            }
            break;
        case RIGHT:
            if (curr_node->dir != LEFT && curr_node->col + 1 < width) {
                (*col)++;
                return true;
            }
            break;
        default:
            break;
    }

    return false;
}

bool is_goal(Node *node) {
    return node->row == GOAL_ROW && node->col == GOAL_COL;
}

void free_pqueue(Node **pqueue, int qsize) {
    for (int i = 0; i < qsize; i++) {
        free(pqueue[i]);
    }
}

void generate_node(char map[MAP_SIZE][MAP_SIZE], int min_g[MAP_SIZE][MAP_SIZE][11][5], int height, int width, Node *pqueue[QUEUE_SIZE], int *qsize, Node *curr_node, Direction dir, int same_dir_count) {
    int new_row = curr_node->row, new_col = curr_node->col;

    if (move(height, width, curr_node, dir, &new_row, &new_col)) {
        int new_g_score = curr_node->g_score + get_cost(map[new_row][new_col]);
        int new_f_score = new_g_score + manhattan_distance(new_row, new_col, GOAL_ROW, GOAL_COL);

        if (new_g_score < min_g[new_row][new_col][same_dir_count][dir]) {
            min_g[new_row][new_col][same_dir_count][dir] = new_g_score;
            Node *new_node = init_node(dir, same_dir_count, new_row, new_col, new_g_score, new_f_score, curr_node);
            push(pqueue, qsize, new_node);
        }
    }
}

int a_star(char map[MAP_SIZE][MAP_SIZE], int height, int width) {
    int result = -1;

    int qsize = 0;
    Node *pqueue[QUEUE_SIZE] = {0};
    int min_g[MAP_SIZE][MAP_SIZE][11][5];

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            for (int sdc = 0; sdc < 11; sdc++) {
                for (int d = 0; d < 5; d++) {
                    min_g[r][c][sdc][d] = INT_MAX;
                }
            }
        }
    }

    push(pqueue, &qsize, init_node(NO_DIR, 0, START_ROW, START_COL, 0, manhattan_distance(START_ROW, START_COL, GOAL_ROW, GOAL_COL), NULL));

    while (qsize > 0) {
        Node *curr_node = pop(pqueue, &qsize);

        if (is_goal(curr_node) && curr_node->same_dir_count >= SAME_DIR_MINIMUM) {
            result = curr_node->g_score;
            free(curr_node);
            break;
        }

        if (curr_node->dir == NO_DIR || (curr_node->same_dir_count >= SAME_DIR_MINIMUM && curr_node->same_dir_count < SAME_DIR_MAXIMUM)) {
            for (Direction dir = UP; dir <= RIGHT; dir++) {
                generate_node(map, min_g, height, width, pqueue, &qsize, curr_node, dir, curr_node->dir == dir ? curr_node->same_dir_count + 1 : 1);
            }
        } else if (curr_node->same_dir_count >= SAME_DIR_MAXIMUM) {
            switch (curr_node->dir) {
                case UP:
                case DOWN:
                    generate_node(map, min_g, height, width, pqueue, &qsize, curr_node, LEFT, 1);
                    generate_node(map, min_g, height, width, pqueue, &qsize, curr_node, RIGHT, 1);
                    break;
                case LEFT:
                case RIGHT:
                    generate_node(map, min_g, height, width, pqueue, &qsize, curr_node, UP, 1);
                    generate_node(map, min_g, height, width, pqueue, &qsize, curr_node, DOWN, 1);
                    break;
                default:
                    break;
            }
        } else {
            generate_node(map, min_g, height, width, pqueue, &qsize, curr_node, curr_node->dir, curr_node->same_dir_count + 1);
        }

        free(curr_node);
    }

    free_pqueue(pqueue, qsize);

    return result;
}

int solve(char map[MAP_SIZE][MAP_SIZE], int height, int width) {
    return a_star(map, height, width);
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    if (ishard) {
        SAME_DIR_MINIMUM = 4;
        SAME_DIR_MAXIMUM = 10;
    } else {
        SAME_DIR_MINIMUM = 0;
        SAME_DIR_MAXIMUM = 3;
    }

    int height = 0, width = 0;
    char map[MAP_SIZE][MAP_SIZE] = {0};

    parse_map(map, &height, &width);

    START_COL = 0;
    START_ROW = 0;
    GOAL_ROW = height - 1;
    GOAL_COL = width - 1;

    int result = solve(map, height, width);
    printf("%d\n", result);
}
