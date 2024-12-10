#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_LEN 19999
#define DISK_SIZE 9 * INPUT_LEN
#define DELIM '\xFF'

int main() {
    FILE *f = fopen("input.txt", "r");
    char input[INPUT_LEN + 1], disk[DISK_SIZE + 1] = {0};
    fgets(input, sizeof(input), f);

    int cursor = 0, current_block = 1;
    for (int i = 0; i < INPUT_LEN; i++) {
        if (i & 1) memset(disk + cursor, DELIM, input[i]);
        else memset(disk + cursor, current_block++, input[i]);
        cursor += atoi((char[]){input[i], '\0'});
    }
    disk[cursor--] = '\0';

    const char delim[] = {DELIM, '\0'};
    char *tok = strtok(disk, delim);

    for (int i = 0; i < cursor + 1; i++) printf(disk[i] != '\xFF' ? "%d" : "*", disk[i] - 1);
    printf("\n");

    while (tok) {
        tok[0] = disk[cursor];
        printf("tok: %ld\n", tok - disk);
        disk[cursor--] = '\0';
        tok = strtok(NULL, delim);
    }

    for (int i = 0; i < cursor + 1; i++) printf(disk[i] != -1 ? "%d" : "*", disk[i] - 1);
    printf("\n");

    return 0;
}
