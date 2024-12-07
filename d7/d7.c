#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 41
#define NUMS_LEN 850
#define NUMS_LEN 850
#define MAX_FACTORS_LEN 850
#define MAX_FACTOR_LEN 3
#define MAX_NUM_LEN 15

long nums[NUMS_LEN];
int nums_len = 0, factors[NUMS_LEN][MAX_FACTORS_LEN], factors_len[NUMS_LEN] = {0};

char explore(long num, int depth[], int max_depth, char op) {
    double res = num;
    int factor = factors[depth[0]][(max_depth - 1) - depth[1]];
    if (op) res -= factor;
    else res /= factor;

    if (res != floor(res) || res < 0) return 0;

    if (depth[1] == (max_depth - 1)) return res == 0;

    return (
        explore((long)res, (int[]){depth[0], depth[1] + 1}, max_depth, 0) ||
        explore((long)res, (int[]){depth[0], depth[1] + 1}, max_depth, 1)
    );
}

char explore_w_concat(long num, int depth[], int max_depth, char op) {
    double res = num;
    int factor = factors[depth[0]][(max_depth - 1) - depth[1]];
    switch (op) {
        case 0:
            res /= factor;
            break;
        case 1:
            res -= factor;
            break;
        default: {
            char num_str[MAX_NUM_LEN + 1], factor_str[MAX_FACTOR_LEN + 1];
            sprintf(num_str, "%ld", num), sprintf(factor_str, "%d", factor);

            int num_str_len = strlen(num_str), factor_str_len = strlen(factor_str);
            char is_substr = 1;

            for (int i = 1; i <= factor_str_len; i++) {
                is_substr &= (num_str[num_str_len - i] == factor_str[factor_str_len - i]);
                if (!is_substr) return 0;
            }

            char res_str[MAX_NUM_LEN] = "\0";
            strncat(res_str, num_str, num_str_len - factor_str_len);
            res = atof(res_str);
        }
    }

    if (res != floor(res) || res < 0) return 0;

    if (depth[1] == (max_depth - 1)) return res == 0;

    return (
        explore_w_concat((long)res, (int[]){depth[0], depth[1] + 1}, max_depth, 0) ||
        explore_w_concat((long)res, (int[]){depth[0], depth[1] + 1}, max_depth, 1) ||
        explore_w_concat((long)res, (int[]){depth[0], depth[1] + 1}, max_depth, 2)
    );
}

int main() {
    FILE *f = fopen("input.txt", "r");
    char ln[41 + 2];
    while (fgets(ln, sizeof(ln), f)) {
        char delim[] = ": ", *tok = strtok(ln, delim);
        nums[nums_len++] = atol(tok);
        tok = strtok(NULL, delim);

        strcpy(delim, " ");
        while (tok) {
            factors[nums_len - 1][factors_len[nums_len - 1]++] = atoi(tok);
            tok = strtok(NULL, delim);
        }
    }

    long long collector = 0;
    for (int i = 0; i < nums_len; i++) {
        if (
            explore(nums[i], (int[]){i, 0}, factors_len[i], 0) ||
            explore(nums[i], (int[]){i, 0}, factors_len[i], 1)
        ) collector += nums[i];
    }
    printf("%lld\n", collector);

    collector = 0;
    for (int i = 0; i < nums_len; i++) {
        if (
            explore_w_concat(nums[i], (int[]){i, 0}, factors_len[i], 0) ||
            explore_w_concat(nums[i], (int[]){i, 0}, factors_len[i], 1) ||
            explore_w_concat(nums[i], (int[]){i, 0}, factors_len[i], 2)
        ) collector += nums[i];
    }
    printf("%lld\n", collector);
    return 0;
}
