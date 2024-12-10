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

    int blocks[INPUT_LEN][3], blocks_len = 0;
    for (int i = 0; i < c_disk_len; i++) {
        int block_id = c_disk[i], start_i = i;
        while (c_disk[i] == block_id) i++;
        memcpy(blocks[blocks_len], (int[]){start_i, block_id, i - start_i}, sizeof(blocks[blocks_len]));
        blocks_len++;
        i--;
    }

    for (int i = blocks_len - 1; i >= 0; i--) {
        int f_block_id = blocks[i][1], f_block_size = blocks[i][2];

        if (f_block_id == -1) continue;

        for (int j = 0; j < i; j++) {
            int r_block_id = blocks[j][1], r_block_size = blocks[j][2];

            if (r_block_id != -1 || r_block_size < f_block_size) continue;

            int r_block_idx = blocks[j][0];

            for (int k = r_block_idx; k < (r_block_idx + f_block_size); k++) c_disk[k] = f_block_id;
        }
    }


    return 0;
}
