#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LINE_LEN 19
#define INPUT_LEN 500
#define ROW_LEN 101
#define N_ROWS 103
#define BOARD_LEN N_ROWS * ROW_LEN
#define SEP "=, \n"
#define UP(A) ((A) - ROW_LEN < 0 ? ((N_ROWS - 1) * ROW_LEN + (A)) : (A) - ROW_LEN)
#define DOWN(A) ((A) + ROW_LEN >= BOARD_LEN ? (A) % ROW_LEN : (A) + ROW_LEN)
#define LEFT(A) ((A) % ROW_LEN == 0 ? ((A) - 1) + ROW_LEN : (A) - 1)
#define RIGHT(A) (((A) + 1) % ROW_LEN == 0 ? ((A) + 1) - ROW_LEN : (A) + 1)


typedef struct {
    int x;
    int y;
    int v_x;
    int v_y;
} robot_t;

int input_len = 0, board[BOARD_LEN];
robot_t input[INPUT_LEN];

void get_input() {
    FILE *f = fopen("input.txt", "r"); 
    char ln[LINE_LEN + 2];
    while (fgets(ln, sizeof(ln), f)) {
        char *tok = strtok(ln, SEP);
        int buf_len = 0, buf[4];

        while (tok) {
            if ((tok[0] > 47 && tok[0] < 58) || tok[0] == 45)
                buf[buf_len++] = atoi(tok);
            tok = strtok(NULL, SEP);
        }

        input[input_len].x = buf[0];
        input[input_len].y = buf[1];
        board[input[input_len].y * ROW_LEN + input[input_len].x]++;

        input[input_len].v_x = buf[2];
        input[input_len++].v_y = buf[3];
    }
    fclose(f);
}

void tick(robot_t *r) {
    int idx = r->y * ROW_LEN + r->x;
    board[idx]--;
    for (int i = 0; i < abs(r->v_x); i++) {
        if (r->v_x < 0) idx = LEFT(idx);
        else idx = RIGHT(idx);
    }

    for (int i = 0; i < abs(r->v_y); i++) {
        if (r->v_y < 0) idx = UP(idx);
        else idx = DOWN(idx);
    }

    r->x = idx % ROW_LEN;
    r->y = idx / ROW_LEN;
    board[idx]++;
}

void simulate(int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < input_len; j++)
            tick(&input[j]);
}
long get_score() {
    long q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (int i = 0; i < BOARD_LEN; i++) {
        if (i / ROW_LEN < N_ROWS / 2 && i % ROW_LEN < ROW_LEN / 2) q1+= board[i];
        else if (i / ROW_LEN < N_ROWS / 2 && i % ROW_LEN > ROW_LEN / 2) q2+= board[i];
        else if (i / ROW_LEN > N_ROWS / 2 && i % ROW_LEN < ROW_LEN / 2) q3+= board[i];
        else if (i / ROW_LEN > N_ROWS / 2 && i % ROW_LEN > ROW_LEN / 2) q4+= board[i];
    }
    return q1 * q2 * q3 * q4;
}

int main() {
    get_input();
    simulate((int)1e2);
    printf("%ld\n", get_score());
    return 0;
}
