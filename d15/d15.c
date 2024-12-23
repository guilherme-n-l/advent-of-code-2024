#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BOARD_DIV "##################################################\n"
#define ROW_LEN 48
#define ROW_LEN2 (2 * ROW_LEN)
#define N_ROWS 48
#define BOARD_LEN ROW_LEN * N_ROWS
#define BOARD_LEN2 (2 * BOARD_LEN)
#define LINE_LEN (int)1e3
#define INSTRUCTIONS_LEN LINE_LEN * 20

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define WALL -1
#define EMPTY 0
#define ROBOT 1
#define BOX 2
#define BOX2 3

#define GPS_SCORE(A, B) (\
        100 * ((A / (B == 1 ? ROW_LEN : ROW_LEN2)) + 1)\
        + (A % (B == 1 ? ROW_LEN : ROW_LEN2))\
        + (B == 1 ? 1 : 2))

#define INIT_BOX(A, B, C) C.l = B[A] == BOX ? A : get_neigh(A, LEFT, 2); \
                                C.r = B[A] == BOX2 ? A : get_neigh(A, RIGHT, 2)

#define CMP_BOX(A, B) (A.l == B.l && A.r == B.r)

typedef struct {
    int l;
    int r;
} box_t;

int board_len = 0, instructions_len = 0, pos0;
char board[BOARD_LEN], instructions[INSTRUCTIONS_LEN];

int get_neigh(int idx, char dir, char pt) {
    int rl = pt == 1 ? ROW_LEN : ROW_LEN2, bl = pt == 1 ? BOARD_LEN : BOARD_LEN2;
    switch (dir) {
        case UP: return ((idx) - rl < 0 ? -1 : (idx) - rl);
        case DOWN: return ((idx) + rl >= bl ? -1 : (idx) + rl);
        case LEFT: return ((idx) % rl == 0 ? -1 : (idx) - 1);
        case RIGHT: return (((idx) + 1) % rl == 0 ? -1 : (idx) + 1);
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
                        pos0 = board_len;
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

char move(int idx, int *robot, char dir, char pt, char b[]) {
    int neigh = get_neigh(idx, dir, pt);

    if (neigh == -1 || b[neigh] == WALL)
        return 0;

    if (b[neigh] == EMPTY || move(neigh, robot, dir, pt, b)) {
        if (idx == *robot) *robot = neigh;
        b[neigh] = b[idx];
        b[idx] = EMPTY;

        return 1;
    }

    return 0;
}

char move_alt(int idx, int *robot, char dir, char b[]) {
    int neigh = get_neigh(idx, dir, 2);

    if (neigh == -1 || b[neigh] == WALL)
        return 0;

    if (b[neigh]) {
        int boxes_len = 0, boxes_bak_len = 0;
        box_t boxes[BOARD_LEN2], boxes_bak[BOARD_LEN2];
        INIT_BOX(neigh, b, boxes[boxes_len]), INIT_BOX(neigh, b, boxes_bak[boxes_len]);
        boxes_len++, boxes_bak_len++;

        while (boxes_len) {
            box_t box = boxes[(boxes_len--) - 1];

            for (int i = 0; i < 2; i++) {
                int neigh = get_neigh(!i ? box.l : box.r, dir, 2);

                if (neigh == -1 || b[neigh] == WALL)
                    return 0;

                if (b[neigh] == EMPTY) continue;

                INIT_BOX(neigh, b, boxes[boxes_len]), INIT_BOX(neigh, b, boxes_bak[boxes_bak_len]);

                char exists = 0;
                for (int i = 0; i < boxes_bak_len; i++)
                    if (CMP_BOX(boxes_bak[i], boxes[boxes_len])) {
                        exists = 1;
                        break;
                    }

                if (exists) continue;

                boxes_len++, boxes_bak_len++;
            }
        }

        for (int i = 0; i < boxes_bak_len; i++)
            for (int j = 0; j < 2; j++)
                b[!j ? boxes_bak[i].l : boxes_bak[i].r] = EMPTY;

        for (int i = 0; i < boxes_bak_len; i++)
            for (int j = 0; j < 2; j++)
                b[get_neigh(!j ? boxes_bak[i].l : boxes_bak[i].r, dir, 2)] = !j ? BOX : BOX2;
    }

    *robot = neigh;
    b[neigh] = ROBOT;
    b[idx] = EMPTY;

    return 1;
}

void init_phase2(char b[], int *robot) {
    int len = 0;

    for (int i = 0; i < BOARD_LEN; i++) {
        if (board[i] == ROBOT) *robot = len;
        b[len++] = board[i];
        switch (board[i]) {
            case BOX:
                b[len++] = BOX2;
                break;
            case ROBOT:
                b[len++] = EMPTY;
                break;
            default:
                b[len++] = board[i];
        }
    }
}

int main() {
    get_input();
    
    int robot_cursor = pos0;

    char pt1[BOARD_LEN];
    memcpy(pt1, board, sizeof(pt1));

    for (int i = 0; i < instructions_len; i++)
        move(robot_cursor, &robot_cursor, instructions[i], 1, pt1);

    long collector = 0;
    for (int i = 0; i < BOARD_LEN; i++) if (pt1[i] == BOX) collector += GPS_SCORE(i, 1);

    printf("%ld\n", collector);

    char pt2[BOARD_LEN2];
    init_phase2(pt2, &robot_cursor);

    for (int i = 0; i < instructions_len; i++) {
        if (instructions[i] < LEFT) move_alt(robot_cursor, &robot_cursor, instructions[i], pt2);
        else move(robot_cursor, &robot_cursor, instructions[i], 2, pt2);
    }

    collector = 0;
    for (int i = 0; i < BOARD_LEN2; i++)
        if (pt2[i] == BOX)
            collector += GPS_SCORE(i, 2);

    printf("%ld\n", collector);

    return 0;
}
