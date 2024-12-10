#include <stdio.h>
#include <string.h>
#define INPUT_LEN 19999
#define DISK_SIZE (9 * INPUT_LEN)

int main() {
    FILE *f = fopen("input.txt", "r");

    char input[INPUT_LEN + 1];
    fgets(input, sizeof(input), f);

    int cursor = 0, current_block = 0, disk[DISK_SIZE];
    for (int i = 0; i < INPUT_LEN; i++) {
        int b_size = input[i] - '0';
        if (i & 1) for (int j = 0; j < b_size; j++) disk[cursor + j] = -1;
        else {
            for (int i = 0; i < b_size; i++) disk[cursor + i] = current_block;
            current_block++;
        }

        cursor += b_size;
    }

    int c_disk[DISK_SIZE], c_disk_len = cursor;

    memcpy(c_disk, disk, sizeof(c_disk));

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

    for (int i = c_disk_len - 1; i >= 0;) {
        if (c_disk[i] == -1) {
            i--;
            continue;
        }

        int b_id = c_disk[i], b_size = 1;
        while (c_disk[--i] == b_id) b_size++;

        for (int j = 0; j <= i; j++) {
            if (c_disk[j] != -1) continue;

            int r_start = j, r_b_size = 1;
            while (c_disk[++j] == -1) r_b_size++;

            if (r_b_size >= b_size) {
                for (int k = 0; k < b_size; k++) c_disk[r_start + k] = b_id;
                for (int k = 0; k < b_size; k++) c_disk[i + 1 + k] = -1;
                break;
            }
        }
    }

    collector = 0;
    for (int i = 0; i <= c_disk_len; i++) {
        if (c_disk[i] == -1) continue;
        collector += i * c_disk[i];
    }

    printf("%ld\n", collector);

    return 0;
}
