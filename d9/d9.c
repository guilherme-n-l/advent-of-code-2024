#include <stdio.h>
#define INPUT_LEN 19999
#define DISK_SIZE (9 * INPUT_LEN)

int main() {
    FILE *f = fopen("input.txt", "r");

    char input[INPUT_LEN + 1];
    fgets(input, sizeof(input), f);

    int cursor = 0, current_block = 0, disk[DISK_SIZE];
    for (int i = 0; i < INPUT_LEN; i++) {
        int b_size = input[i] - '0';
        if (i & 1) for (int i = 0; i < b_size; i++) disk[cursor + i] = -1;
        else {
            for (int i = 0; i < b_size; i++) disk[cursor + i] = current_block;
            current_block++;
        }

        cursor += b_size;
    }

    int b_cursor = cursor - 1;
    cursor = 0;

    while (cursor < b_cursor) {
        if (disk[cursor] != -1) {
            cursor++;
            continue;
        }

        if (disk[b_cursor] == -1) {
            b_cursor--;
            continue;
        }

        disk[cursor++] = disk[b_cursor--];
    }

    long collector = 0;
    for (int i = 0; i <= cursor; i++) collector += i * disk[i];

    printf("%ld\n", collector);

    return 0;
}
