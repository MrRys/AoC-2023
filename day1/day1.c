#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE 100

bool isspelleddigit(char *line, int idx)
{
    if (line == NULL || idx >= MAX_LINE)
        return false;

    return !strncmp(line + idx, "one", 3) ||
           !strncmp(line + idx, "two", 3) ||
           !strncmp(line + idx, "three", 5) ||
           !strncmp(line + idx, "four", 4) ||
           !strncmp(line + idx, "five", 4) ||
           !strncmp(line + idx, "six", 3) ||
           !strncmp(line + idx, "seven", 5) ||
           !strncmp(line + idx, "eight", 5) ||
           !strncmp(line + idx, "nine", 4);
}

int parsedigit(char *line, int idx)
{
    if (line == NULL || idx >= MAX_LINE)
        return 0;

    if (isdigit(line[idx]))
        return line[idx] - '0';

    switch (line[idx])
    {
    case 'o':
        return 1;
    case 't':
        return line[idx + 1] == 'w' ? 2 : 3;
    case 'f':
        return line[idx + 1] == 'o' ? 4 : 5;
    case 's':
        return line[idx + 1] == 'i' ? 6 : 7;
    case 'e':
        return 8;
    case 'n':
        return 9;
    default:
        return 0;
    }
}

int solve(bool ishard)
{
    int result = 0;

    char line[MAX_LINE + 5] = {0};
    while (fgets(line, MAX_LINE, stdin))
    {
        int f = 0;
        while (!isdigit(line[f]) && (!ishard || !isspelleddigit(line, f)))
            f++;

        int l = MAX_LINE - 1;
        while (!isdigit(line[l]) && (!ishard || !isspelleddigit(line, l)))
            l--;

        result += parsedigit(line, f) * 10 + parsedigit(line, l);

        memset(line, 0, MAX_LINE);
    }

    return result;
}

int main(int argc, char *argv[])
{
    bool ishard = argc > 1 && argv[1][0] == 'h';

    int result = solve(ishard);
    printf("%d\n", result);
}