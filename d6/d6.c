#include <stdio.h>
#include <string.h>

#define LINE_LEN 130
#define ARR_LEN 16900

#define UP(A) (A - LINE_LEN < 0 ? -1 : A - LINE_LEN)
#define DOWN(A) (A + LINE_LEN >= ARR_LEN ? -1 : A + LINE_LEN)
#define LEFT(A) ((A) % LINE_LEN == 0 ? -1 : A - 1)
#define RIGHT(A) ((A + 1) % LINE_LEN == 0 ? -1 : A + 1)
#define TURN(A) ((A + 1) % 4)


int move(char d, int c) {
    switch (d) {
        case 0:
            return UP(c);
        case 1:
            return RIGHT(c);
        case 2:
            return DOWN(c);
        case 3:
            return LEFT(c);
    }
    return -1;
}

char is_loop(char d, int c, char arr[]) {
    char ds[ARR_LEN] = {0};

    while (1) {
        if (move(d, c) == -1) break;

        if (ds[c] == (d + 1)) return 1;

        ds[c] = d + 1;
        while(arr[move(d, c)] == -1) d = TURN(d);
        c = move(d, c);
    }
    return 0;
}

int main() {
    FILE *f = fopen("input.txt", "r");
    char ln[LINE_LEN + 2], arr[ARR_LEN] = {0}, original_arr[ARR_LEN] = {0};
    int arr_len = 0, start_position;
    while (fgets(ln, sizeof(ln), f)) {
        for (int i = 0; i < LINE_LEN; i++) {
            switch (ln[i]) {
                case '#':
                    original_arr[arr_len++] = -1;
                    break;
                case '^':
                    start_position = arr_len;
                default:
                    arr_len++;
            }
        }
    }

    memcpy(arr, original_arr, sizeof(arr));

    char direction = 0;
    int collector = 0, cursor = start_position;
    while (1) {
        if (move(direction, cursor) == -1) break;

        if (!arr[cursor]) {
            arr[cursor] = 1;
            collector++;
        }

        while (arr[move(direction, cursor)] == -1) direction = TURN(direction);

        cursor = move(direction, cursor);
    }
    printf("%d\n", collector + 1);

    int visited[ARR_LEN], visited_len = 0;
    for (int i = 0; i < arr_len; i++) if (arr[i] == 1) visited[visited_len++] = i;

    collector = 0;

    for (int i = 0; i < visited_len; i++) {
        char c_arr[ARR_LEN];
        memcpy(c_arr, original_arr, sizeof(c_arr));
        c_arr[visited[i]] = -1;
        if (is_loop(0, start_position, c_arr)) collector++;
    }

    printf("%d\n", collector);
    return 0;
}
