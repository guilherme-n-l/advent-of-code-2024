#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 36
#define REPEAT 25
#define REPEAT_PT2 75
#define NUMS_LEN 8
#define POLL(A, B) (A[--B])
#define PILE(A, B, C) (A[B++] = C)

long *blink(long n) {
    static long arr[2];

    char num_str[250];

    switch (n) {
        case 0:
            arr[0] = 1, arr[1] = -1;
            return arr;
        default:
            sprintf(num_str, "%ld", n); 
            if (strlen(num_str) & 1) {
                arr[0] = n * 2024, arr[1] = -1;
                return arr;
            }
            else {
                char slice[125];

                for (int k = 0; k < 2; k++) {
                    int half_size = strlen(num_str) / 2;
                    strncpy(slice, num_str + k * half_size, half_size);
                    slice[half_size] = '\0';
                    arr[k] = atol(slice);
                }

                return arr;
            }
    }
}

int main() {
    FILE *f = fopen("input.txt", "r");
    char ln[LINE_LEN + 2];
    fgets(ln, sizeof(ln), f);
    
    char *tok = strtok(ln, " ");
    int nums[NUMS_LEN], nums_len = 0;

    while (tok) {
        nums[nums_len++] = atoi(tok);
        tok = strtok(NULL, " ");
    }

    int depths_len = 0, n_len = 0;
    long depths[nums_len + REPEAT_PT2], n[nums_len + REPEAT_PT2], collector = 0;

    for (int i = 0; i < nums_len; i++) {
        PILE(depths, depths_len, 0);
        PILE(n, n_len, nums[i]);
    }

    while (n_len > 0) {
        long num = POLL(n, n_len), depth = POLL(depths, depths_len);

        if (depth == REPEAT) {
            collector++;
            continue;
        }
        
        long *blinked = blink(num);
        for (int i = 0; i < 2; i++) if (blinked[i] != -1) {
            PILE(depths, depths_len, depth + 1);
            PILE(n, n_len, blinked[i]);
        }
    }

    printf("%ld\n", collector);

    depths_len = 0, n_len = 0;
    collector = 0;

    for (int i = 0; i < nums_len; i++) {
        PILE(depths, depths_len, 0);
        PILE(n, n_len, nums[i]);
    }

    while (n_len > 0) {
        long num = POLL(n, n_len), depth = POLL(depths, depths_len);

        if (depth == REPEAT_PT2) {
            collector++;
            continue;
        }
        
        long *blinked = blink(num);
        for (int i = 0; i < 2; i++) if (blinked[i] != -1) {
            PILE(depths, depths_len, depth + 1);
            PILE(n, n_len, blinked[i]);
        }
    }

    printf("%ld\n", collector);

    return 0;
}
