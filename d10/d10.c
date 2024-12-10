#include <stdio.h>

#define LINE_LEN 45
#define INPUT_LEN (LINE_LEN * LINE_LEN)
#define UP(A) (A - LINE_LEN < 0 ? -1 : A - LINE_LEN)
#define DOWN(A) (A + LINE_LEN >= INPUT_LEN ? -1 : A + LINE_LEN)
#define LEFT(A) ((A) % LINE_LEN == 0 ? -1 : A - 1)
#define RIGHT(A) ((A + 1) % LINE_LEN == 0 ? -1 : A + 1)
#define POLL(A, B) (A[--B])
#define PILE(A, B, C) (A[B++] = C)

int *adjacent(int idx) {
    static int adj[4];
    adj[0] = UP(idx);
    adj[1] = DOWN(idx);
    adj[2] = LEFT(idx);
    adj[3] = RIGHT(idx);
    return adj;
}

int main() {
    FILE *f = fopen("input.txt", "r");

    char ln[LINE_LEN + 2], input[INPUT_LEN];
    int zeros[INPUT_LEN], zeros_len = 0;
    int input_len = 0;
    while (fgets(ln, sizeof(ln), f)) {
        for (int i = 0; i < LINE_LEN; i++) {
            int digit = ln[i] - '0';

            if (!digit) zeros[zeros_len++] = input_len + i;

            input[input_len + i] = digit;
        }

        input_len += LINE_LEN;
    }

    int collector = 0;
    for (int i = 0; i < zeros_len; i++) {
        char visited[INPUT_LEN] = {0};
        int stack[INPUT_LEN], stack_len = 0, *neighs = adjacent(zeros[i]);

        for (int j = 0; j < 4; j++) {
            if (neighs[j] == -1 || input[neighs[j]] != 1) continue;
            PILE(stack, stack_len, neighs[j]);
        }

        while (stack_len > 0) {
            int idx = POLL(stack, stack_len);
            
            if (input[idx] == 9) {
                if (visited[idx]) continue;
                
                visited[idx] = 1;
                collector++;
            }

            neighs = adjacent(idx);

            for (int j = 0; j < 4; j++) {
                if (neighs[j] == -1 || input[neighs[j]] != (input[idx] + 1)) continue;
                PILE(stack, stack_len, neighs[j]);
            }
        }
    }

    printf("%d\n", collector);

    collector = 0;
    for (int i = 0; i < zeros_len; i++) {
        int stack[INPUT_LEN], stack_len = 0, *neighs = adjacent(zeros[i]);

        for (int j = 0; j < 4; j++) {
            if (neighs[j] == -1 || input[neighs[j]] != 1) continue;
            PILE(stack, stack_len, neighs[j]);
        }

        while (stack_len > 0) {
            int idx = POLL(stack, stack_len);
            
            if (input[idx] == 9) {
                collector++;
            }

            neighs = adjacent(idx);

            for (int j = 0; j < 4; j++) {
                if (neighs[j] == -1 || input[neighs[j]] != (input[idx] + 1)) continue;
                PILE(stack, stack_len, neighs[j]);
            }
        }
    }

    printf("%d\n", collector);

    return 0;
}
