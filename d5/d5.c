#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_LNS 70
#define PAIRS_LEN 1176
#define SEQUENCES_LEN 188
#define MAX_SEQUENCE_LEN 23

int pairs[PAIRS_LEN][2], pairs_len = 0;

int compare(int *pair) {
    int int_a = pair[0], int_b = pair[1];
    for (int i = 0; i < pairs_len; i++) {
        if (pairs[i][0] == int_a && pairs[i][1] == int_b) return -1;
        if (pairs[i][1] == int_a && pairs[i][0] == int_b) return 1;
    }
    return 0;
}

int qcompare(const void* a, const void* b) {
    int int_a = *(int *)a, int_b = *(int *)b;
    return compare((int[]){int_a, int_b});
}

int main() {
    FILE *f = fopen("input.txt", "r");

    char ln[N_LNS];
    while (fgets(ln, sizeof(ln), f)) {
        if (strcmp(ln, "\n") == 0) break;

        sscanf(ln, "%d|%d", &pairs[pairs_len][0], &pairs[pairs_len][1]);
        pairs_len++;
    }

    int sequences[SEQUENCES_LEN][MAX_SEQUENCE_LEN], sequences_len = 0,len_sequence[SEQUENCES_LEN] = {0};
    while (fgets(ln, sizeof(ln), f)) {
        if (strcmp(ln, "\n") == 0) break;
        char *tok = strtok(ln, ",");
        while (tok) {
            sequences[sequences_len][len_sequence[sequences_len]++] = atoi(tok);
            tok = strtok(NULL, ",");
        }
        sequences_len++;
    }

    fclose(f);

    int collector = 0, wrong_sequences[SEQUENCES_LEN], wrong_sequences_len = 0;
    for (int i = 0; i < sequences_len; i++) {
        char completed = 1;
        for (int j = 0; j < len_sequence[i] - 1; j++) {

            for (int k = j + 1; k < len_sequence[i]; k++) {
                int pair[] = {sequences[i][j], sequences[i][k]};
                if (compare(pair) == 1) {
                    completed = 0;
                    wrong_sequences[wrong_sequences_len++] = i;
                    break;
                };
            }

            if (!completed) break;
        }

        if (!completed) continue;

        collector += sequences[i][(len_sequence[i] + 1) / 2 - 1];
    }
    printf("%d\n", collector);

    collector = 0;
    for (int i = 0; i < wrong_sequences_len; i++) {
        int idx = wrong_sequences[i];
        qsort(sequences[idx], len_sequence[idx], sizeof(int), qcompare);
        collector += sequences[idx][(len_sequence[idx] + 1) / 2 - 1];
    }

    printf("%d\n", collector);


    return 0;
}
