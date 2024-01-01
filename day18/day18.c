#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_INSTR 660

typedef struct instruction {
    char dir;
    int steps;
} Instruction;

typedef struct point {
    long x;
    long y;
} Point;

void parse_instructions(Instruction *instructions, int *instr_count) {
    *instr_count = 0;

    while (scanf("%c %d (#%*x)\n", &instructions[*instr_count].dir, &instructions[*instr_count].steps) > 0) {
        (*instr_count)++;
    }
}

void parse_instructions_hard(Instruction *instructions, int *instr_count) {
    *instr_count = 0;

    while (scanf("%*c %*d (#%5x%hhd)\n", &instructions[*instr_count].steps, &instructions[*instr_count].dir) > 0) {
        switch (instructions[*instr_count].dir) {
            case 0:
                instructions[*instr_count].dir = 'R';
                break;
            case 1:
                instructions[*instr_count].dir = 'D';
                break;
            case 2:
                instructions[*instr_count].dir = 'L';
                break;
            case 3:
                instructions[*instr_count].dir = 'U';
                break;
        }
        (*instr_count)++;
    }
}

void generate_points(Point *points, Instruction *instructions, int instr_count) {
    points[0].x = points[0].y = 0;

    for (int instr = 0; instr < instr_count; instr++) {
        switch (instructions[instr].dir) {
            case 'R':
                points[instr + 1].x = points[instr].x + instructions[instr].steps;
                points[instr + 1].y = points[instr].y;
                break;
            case 'D':
                points[instr + 1].x = points[instr].x;
                points[instr + 1].y = points[instr].y + instructions[instr].steps;
                break;
            case 'L':
                points[instr + 1].x = points[instr].x - instructions[instr].steps;
                points[instr + 1].y = points[instr].y;
                break;
            case 'U':
                points[instr + 1].x = points[instr].x;
                points[instr + 1].y = points[instr].y - instructions[instr].steps;
                break;
            default:
                break;
        }
    }
}

long shoelace_formula(Point *points, int point_count) {
    long area = 0;

    for (int i = 0; i < point_count; i++) {
        area += points[i].x * (points[(i + 1) % point_count].y - points[(i - 1) % point_count].y);
    }

    return area / 2;
}

long perimeter_formula(Instruction *instructions, int instr_count) {
    long perimeter = 0;

    for (int i = 0; i < instr_count; i++) {
        perimeter += instructions[i].steps;
    }

    return perimeter;
}

long picks_formula(long area, long perimeter) {
    return area - perimeter / 2 + 1;
}

long solve(Instruction *instructions, long instr_count) {
    Point *points = (Point *)malloc((instr_count + 1) * sizeof(Point));
    generate_points(points, instructions, instr_count);

    long area = shoelace_formula(points, instr_count + 1);
    long perimeter = perimeter_formula(instructions, instr_count);
    long inner_area = picks_formula(area, perimeter);

    free(points);

    return inner_area + perimeter;
}

int main(int argc, char *argv[]) {
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int instr_count = 0;
    Instruction instructions[MAX_INSTR] = {0};
    ishard ? parse_instructions_hard(instructions, &instr_count) : parse_instructions(instructions, &instr_count);

    long result = solve(instructions, instr_count);
    printf("%ld\n", result);
}
