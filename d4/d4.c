#include <stdio.h>
#include <string.h>

#define ROW_LEN (140)
#define ARR_LEN (ROW_LEN * ROW_LEN)
#define UP(A) (A - ROW_LEN < 0 ? -1 : A - ROW_LEN)
#define DOWN(A) (A + ROW_LEN >= ARR_LEN ? -1 : A + ROW_LEN)
#define LEFT(A) ((A) % ROW_LEN == 0 ? -1 : A - 1)
#define RIGHT(A) ((A + 1) % ROW_LEN == 0 ? -1 : A + 1)
#define DIAG_A(A) ((DOWN(A) == -1 || LEFT(A) == -1) ? -1 : DOWN(LEFT(A)))
#define DIAG_B(A) ((DOWN(A) == -1 || RIGHT(A) == -1) ? -1 : DOWN(RIGHT(A)))
#define DIAG_C(A) ((UP(A) == -1 || LEFT(A) == -1) ? -1 : UP(LEFT(A)))
#define DIAG_D(A) ((UP(A) == -1 || RIGHT(A) == -1) ? -1 : UP(RIGHT(A)))

int get_index_from_operation(int idx, char op) {
    switch (op) {
        case 0: return UP(idx);
        case 1: return DOWN(idx);
        case 2: return LEFT(idx);
        case 3: return RIGHT(idx);
        case 4: return DIAG_A(idx);
        case 5: return DIAG_B(idx);
        case 6: return DIAG_C(idx);
        case 7: return DIAG_D(idx);
    }
    return -1;
}

int main() {
    FILE *f = fopen("input.txt", "r");
    int arr_len = 0, x_arr[3680], x_arr_len = 0;
    char ln[ROW_LEN + 2], arr[ARR_LEN];
    while (fgets(ln, sizeof(ln), f)) {
        for (int i = 0; i < ROW_LEN; i++) {
            arr[arr_len++] = ln[i];
            if (ln[i] == 'X') x_arr[x_arr_len++] = arr_len - 1;
        }
    } 
    fclose(f);

    int collector = 0;
    for (int i = 0; i < x_arr_len; i++) {
        for (char j = 0; j < 8; j++) {

            char word[4 + 1] = {0};
            word[0] = 'X', word[4] = '\0';

            int cursor = x_arr[i];
            for (char k = 0; k < 3; k++) {
                cursor = get_index_from_operation(cursor, j);
                if (cursor == -1) break;

                word[k + 1] = arr[cursor];
            }

            if (strcmp(word, "XMAS") == 0) collector++;
        }
    }

    printf("%d\n", collector);
    return 0;
}
