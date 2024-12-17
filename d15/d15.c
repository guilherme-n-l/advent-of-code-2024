#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BOARD_DIV "##################################################\n"
#define ROW_LEN 48
#define N_ROWS 48
#define BOARD_LEN ROW_LEN * N_ROWS
#define LINE_LEN (int)1e3
#define INSTRUCTIONS_LEN LINE_LEN * 20

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define UP_OP(A) ((A) - ROW_LEN < 0 ? -1 : (A) - ROW_LEN)
#define DOWN_OP(A) ((A) + ROW_LEN >= BOARD_LEN ? -1 : (A) + ROW_LEN)
#define LEFT_OP(A) ((A) % ROW_LEN == 0 ? -1 : (A) - 1)
#define RIGHT_OP(A) (((A) + 1) % ROW_LEN == 0 ? -1 : (A) + 1)

#define WALL -1
#define EMPTY 0
#define ROBOT 1
#define BOX 2

#define GPS_SCORE(A) (100 * ((A / ROW_LEN) + 1) + (A % ROW_LEN) + 1)


int board_len = 0, instructions_len = 0, robot_idx;
char board[BOARD_LEN], instructions[INSTRUCTIONS_LEN];

int get_neigh(int idx, char dir) {
    switch (dir) {
        case UP: return UP_OP(idx);
        case DOWN: return DOWN_OP(idx);
        case LEFT: return LEFT_OP(idx);
        case RIGHT: return RIGHT_OP(idx);
        default: abort();
    }
}

void get_input() {
    FILE *f = fopen("input.txt" , "r");
    char ln[LINE_LEN + 2];
    while (fgets(ln, sizeof(ln), f)) {
        if (ln[0] == '\n') continue;

        if (ln[0] == '#') {
            if (!strcmp(BOARD_DIV, ln)) continue;

            for (int i = 0; i < ROW_LEN; i++)
                switch (ln[i + 1]) {
                    case '#': board[board_len++] = WALL; break;
                    case '@': {
                        robot_idx = board_len;
                        board[board_len++] = ROBOT;
                        break;
                    }
                    case 'O': board[board_len++] = BOX; break;
                    default: board[board_len++] = EMPTY;
                }

            continue;
        }

        for (int i = 0; i < LINE_LEN; i++)
            switch (ln[i]) {
                case '^': instructions[instructions_len++] = UP; break;
                case 'v': instructions[instructions_len++] = DOWN; break;
                case '<': instructions[instructions_len++] = LEFT; break;
                case '>': instructions[instructions_len++] = RIGHT;
            }
    }

    fclose(f);
}

char move(int idx, char dir) {
    int neigh = get_neigh(idx, dir);

    if (neigh == -1 || board[neigh] == WALL)
        return 0;

    if (board[neigh] == EMPTY || move(neigh, dir)) {
        if (idx == robot_idx) robot_idx = neigh;
        board[neigh] = board[idx];
        board[idx] = EMPTY;

        return 1;
    }

    return 0;
}

int main() {
    get_input();
    
    for (int i = 0; i < instructions_len; i++)
        move(robot_idx, instructions[i]);

    for (int i = 0; i < BOARD_LEN; i++) {
        switch (board[i]) {
            case WALL: printf("#"); break;
            case ROBOT: printf("@"); break;
            case BOX: printf("O"); break;
            default: printf(".");
        }

        if (!((i + 1) % ROW_LEN)) printf("\n");
    }

    long collector = 0;
    for (int i = 0; i < BOARD_LEN; i++) if (board[i] == BOX) collector += GPS_SCORE(i);

    printf("%ld\n", collector);

    return 0;
}
