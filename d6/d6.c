#include <stdio.h>

#define LINE_LEN 130
#define ARR_LEN 16900

#define UP(A) (A - LINE_LEN < 0 ? -1 : A - LINE_LEN)
#define DOWN(A) (A + LINE_LEN >= ARR_LEN ? -1 : A + LINE_LEN)
#define LEFT(A) ((A) % LINE_LEN == 0 ? -1 : A - 1)
#define RIGHT(A) ((A + 1) % LINE_LEN == 0 ? -1 : A + 1)
#define TURN(A) ((A + 1) % 4)

int cursor;
char direction = 0;

int move() {
    switch (direction) {
        case 0:
            return UP(cursor);
        case 1:
            return RIGHT(cursor);
        case 2:
            return DOWN(cursor);
        case 3:
            return LEFT(cursor);
    }
    return -1;
}

int main() {
    FILE *f = fopen("input.txt", "r");
    char ln[LINE_LEN + 2], arr[ARR_LEN] = {0};
    int arr_len = 0;
    while (fgets(ln, sizeof(ln), f)) {
        for (int i = 0; i < LINE_LEN; i++) {
            switch (ln[i]) {
                case '#':
                    arr[arr_len++] = -1;
                    break;
                case '^':
                    cursor = arr_len;
                default:
                    arr_len++;
            }
        }
    }

    int collector = 0;
    while (cursor != -1) {
        if (arr[move()] == -1) direction = TURN(direction);
        if (!arr[cursor]) {
            arr[cursor] = 1;
            collector++;
        }
        cursor = move();
    }

    printf("%d\n", collector);

    return 0;
}
